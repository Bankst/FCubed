namespace TeensyFFComms.Monitoring
{
	public class USBDevice
	{
		public USBDeviceType   Type;
		public uint   SerialNumber { get; set; }
		public string Port         { get; set; }

        public override string ToString()
        {
            var hasSerial = Type == USBDeviceType.UsbSerial;
            return $"Type: {Type} {(hasSerial ? $"COM Port: {Port}" :"No COM Port")}, SerialNo: {SerialNumber}";
        }
    }
}