using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;

namespace TeensyFFComms.Monitoring
{
    public class TeensyWatcher : IDisposable
    {
		private const uint Vid = 0x16C0;
		private const uint SerPid = 0x483;
		private const uint HIDPid = 0x486;
		private const uint HalfKayPid = 0x478;
		private readonly string _vidStr = $"'%USB_VID[_]{Vid:X}%'";

        #region Properties and Events -----------------------------------------------

        public List<USBDevice> ConnectedDevices { get; private set; }
        public event EventHandler<ConnectionChangedEventArgs> ConnectionChanged;

        #endregion

        #region Construction / Destruction ------------------------------------------

        public TeensyWatcher()
        {
            ConnectedDevices = new List<USBDevice>();

            using (var searcher = new ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PnPEntity WHERE DeviceID LIKE " + _vidStr))
            {
                foreach (var mgmtObject in searcher.Get())
                {
                    var device = MakeDevice(mgmtObject);
                    if (device != null)
                    {
                        ConnectedDevices.Add(device);
                    }
                }
            }
            StartWatching();
        }

        public void Dispose()
        {
            StopWatching();
        }

        #endregion

        #region Port Watching  ------------------------------------------------------

        protected ManagementEventWatcher CreateWatcher;
        protected ManagementEventWatcher DeleteWatcher;

        protected void StartWatching()
        {
            StopWatching(); // Just to make sure 

            DeleteWatcher = new ManagementEventWatcher
            {
                Query = new WqlEventQuery
                {
                    EventClassName = "__InstanceDeletionEvent",
                    Condition = "TargetInstance ISA 'Win32_PnPEntity'",
                    WithinInterval = new TimeSpan(0, 0, 1), //Todo: make the interval settable
                },
            };
            DeleteWatcher.EventArrived += PortsChanged;
            DeleteWatcher.Start();

            CreateWatcher = new ManagementEventWatcher
            {
                Query = new WqlEventQuery
                {
                    EventClassName = "__InstanceCreationEvent",
                    Condition = "TargetInstance ISA 'Win32_PnPEntity'",
                    WithinInterval = new TimeSpan(0, 0, 1), //Todo: make the interval settable
                },
            };
            CreateWatcher.EventArrived += PortsChanged;
            CreateWatcher.Start();
        }

        protected void StopWatching()
        {
            if (CreateWatcher != null)
            {
                CreateWatcher.Stop();
                CreateWatcher.Dispose();
            }

			if (DeleteWatcher == null) return;
			DeleteWatcher.Stop();
			DeleteWatcher.Dispose();
		}

        public enum ChangeType
        {
            Add,
            Remove
        }

		private void PortsChanged(object sender, EventArrivedEventArgs e)
        {
            var device = MakeDevice((ManagementBaseObject)e.NewEvent["TargetInstance"]);
			if (device == null) return;
			var type = e.NewEvent.ClassPath.ClassName == "__InstanceCreationEvent" ? ChangeType.Add : ChangeType.Remove;

			if (type == ChangeType.Add)
			{
				ConnectedDevices.Add(device);
				OnConnectionChanged(type, device);
			}
			else
			{
				var rd = ConnectedDevices.Find(d => d.Serialnumber == device.Serialnumber);
				ConnectedDevices.Remove(rd);
				OnConnectionChanged(type, rd);
			}
		}

        #endregion

        #region Helpers

        protected USBDevice MakeDevice(ManagementBaseObject mgmtObj)
        {
            var deviceIdParts = ((string)mgmtObj["PNPDeviceID"]).Split("\\".ToArray());

            if (deviceIdParts[0] != "USB") return null;

            var start = deviceIdParts[1].IndexOf("PID_", StringComparison.Ordinal) + 4;
            var pid = Convert.ToUInt32(deviceIdParts[1].Substring(start, 4), 16);

            switch (pid)
			{
				case SerPid:
				{
					var serNum = Convert.ToUInt32(deviceIdParts[2]);
					var port = (((string)mgmtObj["Caption"]).Split("()".ToArray()))[1];

					return new USBDevice
					{
						Type = USBDeviceType.UsbSerial,
						Port = port,
						Serialnumber = serNum
					};
				}

				case HIDPid:
				{
					var serNum = Convert.ToUInt32(deviceIdParts[2]);
					return new USBDevice
					{
						Type = USBDeviceType.HID,
						Port = "",
						Serialnumber = serNum
					};
				}

                case HalfKayPid:
				{
					var serNum = Convert.ToUInt32(deviceIdParts[2], 16);
					if (serNum != 0xFFFFFFFF)
					{
						serNum *= 10;
					}

					return new USBDevice
					{
						Type = USBDeviceType.HalfKay,
						Port = "",
						Serialnumber = serNum,
					};
				}

				default:
					return null;
			}
		}

        #endregion

        #region EventHandler --------------------------------------------------------

        protected void OnConnectionChanged(ChangeType type, USBDevice changedDevice)
		{
			ConnectionChanged?.Invoke(this, new ConnectionChangedEventArgs(type, changedDevice));
		}

        #endregion
    }

	public class ConnectionChangedEventArgs : EventArgs
    {
        public readonly TeensyWatcher.ChangeType ChangeType;
        public readonly USBDevice ChangedDevice;

        public ConnectionChangedEventArgs(TeensyWatcher.ChangeType type, USBDevice changedDevice)
        {
            ChangeType = type;
            ChangedDevice = changedDevice;
        }
    }
}
