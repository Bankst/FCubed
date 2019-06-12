namespace TeensyFFComms.Monitoring
{
	public class USBDevice
	{
		public USBDeviceType   Type;
		public uint   Serialnumber { get; set; }
		public string Port         { get; set; }
	}
}