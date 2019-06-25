using ProtoBuf;

using System;
using System.IO;
using System.IO.Ports;
using System.Threading;

using TeensyFFComms.Monitoring;
using TeensyFFComms.Protocols;

namespace TeensyFFCommsConsole
{
    internal class Program
    {

        public const int UnitSerialNumber = 4037510;
        public const int ControlBaud = 500000;

        private static TeensyWatcher _watcher;
        private static USBDevice _mainCpu;
        private static readonly bool Connected = _mainCpu != null;
        private static SerialPort _mainCpuPort = new SerialPort();

		private static void Main(string[] args)
        {
            _watcher = new TeensyWatcher(250);
            _watcher.ConnectionChanged += WatcherOnConnectionChanged;
            Console.ReadLine();
        }

        private static void _mainCpuPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var sp = (SerialPort)sender;
            var indata = sp.ReadExisting();
        }

        // TODO: abstractify ?
        private static void WatcherOnConnectionChanged(object sender, ConnectionChangedEventArgs eventArgs)
        {
            if (eventArgs.ChangedDevice.SerialNumber != UnitSerialNumber)
            {
                Console.WriteLine($"Ignoring event of device with undesired serial: {eventArgs.ChangedDevice.SerialNumber}");
                return;
            }

            Console.Write($"{eventArgs} ~ ");

            switch (eventArgs.ChangeType)
            {
                case TeensyWatcher.ChangeType.Add:
                    if (!Connected)
                    {
                        _mainCpu = eventArgs.ChangedDevice;
                        Console.WriteLine("added");

                        try
                        {
                            _mainCpuPort = new SerialPort(_mainCpu.Port, ControlBaud) {DtrEnable = true};
                            _mainCpuPort.DataReceived += _mainCpuPort_DataReceived;
                            _mainCpuPort.Open();
                            Console.WriteLine("Serial port opened at 4.5Mb/s! Sending init message after 500ms delay.");
                            Thread.Sleep(500);

                            var buf = new MemoryStream();
                            var msg = new InitReq();
                            
                            Serializer.Serialize(buf, msg);

                            var sendBytes = new byte[buf.Length + 1];
                            sendBytes[0] = (byte)F3Opcode.InitReq;
                            Buffer.BlockCopy(buf.GetBuffer(), 0, sendBytes, 1, (int) buf.Length);

                            _mainCpuPort.Write(sendBytes, 0, sendBytes.Length);
                            Console.WriteLine($"Sent message. Length: {sendBytes.Length}, Opcode: {msg.Opcode}");

                        }
                        catch (Exception e)
                        {
                            Console.WriteLine($"Failed to open serial port: \n {e.Message}");
                        }

                    }
                    else
                    {
                        Console.WriteLine("exists");
                    }
                    break;
                case TeensyWatcher.ChangeType.Remove:
                    Console.WriteLine(Connected ? "removed" : "already removed");
                    _mainCpu = null;
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }
}
