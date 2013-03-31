using System;
using System.IO.Ports;

namespace Firmware_Updater
{
	public class FirmwareLink
	{
		public static void WritePage(byte[] pageData, int pageNumber, int pageSize, SerialPort port)
		{
			port.Write(new byte[1]{Convert.ToByte(pageNumber)},0,1);
			port.Write(pageData,pageNumber*pageSize,pageSize);
			if(port.ReadChar()!='A')
				throw new Exception("No acknowledge from target!");
		}
	}
}
