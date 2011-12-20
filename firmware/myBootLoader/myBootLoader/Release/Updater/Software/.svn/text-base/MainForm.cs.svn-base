using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;

namespace Firmware_Updater
{
	public partial class MainForm : Form
	{
		int flashSize=1024;
		byte[] memMap;
		int memLength=-1;
		
		[STAThread]
		public static void Main(string[] args)
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
		}
		public MainForm()
		{
			InitializeComponent();
		}
		void UpdatePortList()
		{
			comboBox1.Items.Clear();
			comboBox1.Items.AddRange(SerialPort.GetPortNames());
			comboBox1.Text=comboBox1.Items[comboBox1.Items.Count-1].ToString();
		}
		void Button1Click(object sender, EventArgs e)
		{
			Browse();
		}
		void Browse()
		{
			if(openFileDialog1.ShowDialog()==DialogResult.OK)
			{
				textBox1.Text=openFileDialog1.FileName.Replace('\\','/');
				UpdateNotifyIconText();
			}
		}
		void MainFormLoad(object sender, EventArgs e)
		{
			UpdatePortList();
			UpdateNotifyIconText();
		}
		void print(string s)
		{
			richTextBox1.AppendText(s+"\n");
			Application.DoEvents();
		}
		void Button2Click(object sender, EventArgs e)
		{
			UpdateTarget();
		}
		bool UpdateTarget()
		{
			bool reply=true;
			SerialPort port=null;
			try
			{
				port=new SerialPort(comboBox1.Text,19200);
				port.ReadTimeout=1000;
				port.Open();
				if(!port.IsOpen)
					throw new Exception("Couldn't open port!");
				print("Port opened");
				
				if(textBox1.Text=="")
					throw new Exception("Pick a file first");
				
				if(textBox1.Text.EndsWith(".gen",true,System.Globalization.CultureInfo.CurrentUICulture))
					ReadFileGeneric(textBox1.Text);
				if(textBox1.Text.EndsWith(".hex",true,System.Globalization.CultureInfo.CurrentUICulture))
					ReadFileHex(textBox1.Text);
				else
					throw new Exception("Unknown file format!");
				
				print("File read");
				
				port.Write(new char[]{'B'},0,1);
				int appAddress=port.ReadByte();
				appAddress+=port.ReadByte()*256;
				appAddress*=2;
				int pageSize=port.ReadByte()*2;
				if((appAddress%pageSize)!=0)
					throw new Exception("Application doesn't start on a page boundary!");
				print("Application address: "+appAddress.ToString());
				
				print("Programming...");
				int lastPage=flashSize/pageSize;
				if((flashSize%pageSize)>0)
					lastPage++;
				progressBar1.Value=0;
				progressBar1.Maximum=lastPage-(appAddress/pageSize);
				for(int memPage=(appAddress/pageSize);memPage<lastPage;memPage++)
				{
					print("  Page "+memPage.ToString());
					FirmwareLink.WritePage(memMap,memPage,pageSize,port);
					progressBar1.PerformStep();
				}
				progressBar1.Value=0;
				print("Done :)");
			}
			catch(Exception ex)
			{
				print(ex.Message);
				reply=false;
			}
			
			if(port!=null)
			{
				if(port.IsOpen)
				{
					port.Close();
					print("Port closed");
				}
			}
			return reply;
		}
		bool Ping()
		{
			bool reply=true;
			SerialPort port=null;
			
			try
			{
				port=new SerialPort(comboBox1.Text,19200);
				port.ReadTimeout=1000;
				port.Open();
				if(!port.IsOpen)
					throw new Exception("Couldn't open port!");
				print("Port opened");
				
				print("Ping?");
				port.Write(new char[]{'?'},0,1);
				string s=port.ReadLine();
				//print("Got "+s);
				print("Pong!");
			}
			catch(Exception ex)
			{
				print(ex.Message);
				reply=false;
			}
			
			if(port!=null)
			{
				if(port.IsOpen)
				{
					port.Close();
					print("Port closed");
				}
			}
			return reply;
		}
		void ReadFileGeneric(string filename)
		{
			memMap=new byte[flashSize];
			for(int i=0;i<memMap.Length;i++)
				memMap[i]=0xff;
			memLength=-1;
			FileStream fs=new FileStream(filename, FileMode.Open);
			TextReader tr=new StreamReader(fs);
			string s;
			int a,d;
			while((s=tr.ReadLine()) != null)
			{
				a=int.Parse(s.Substring(0,6), System.Globalization.NumberStyles.AllowHexSpecifier);
				d=int.Parse(s.Substring(7,4), System.Globalization.NumberStyles.AllowHexSpecifier);
				if((2*a)>memLength)
					memLength=2*a;
				memMap[2*a]=Convert.ToByte(d&0xff);
				memMap[2*a+1]=Convert.ToByte(d>>8);
			}
			tr.Close();
			fs.Close();
			if(memLength>=flashSize)
				throw new Exception("Program is larger than flash!");
		}
		void ReadFileHex(string filename)
		{
			memMap=new byte[flashSize];
			for(int i=0;i<memMap.Length;i++)
				memMap[i]=0xff;
			memLength=-1;
			FileStream fs=new FileStream(filename, FileMode.Open);
			TextReader tr=new StreamReader(fs);
			string s;
			int c,a,t;
			while((s=tr.ReadLine()) != null)
			{
				if(s[0]!=':')
					throw new Exception("Error in file or not a hex file!");
				c=int.Parse(s.Substring(1,2), System.Globalization.NumberStyles.AllowHexSpecifier);
				a=int.Parse(s.Substring(3,4), System.Globalization.NumberStyles.AllowHexSpecifier);
				t=int.Parse(s.Substring(7,2), System.Globalization.NumberStyles.AllowHexSpecifier);
				switch(t)
				{
					case 0:
						for(int i=9,o=0;i<s.Length-3;i+=2,o++)
							memMap[a+o]=byte.Parse(s.Substring(i,2), System.Globalization.NumberStyles.AllowHexSpecifier);
						break;
					case 1:
						break;
					//default:
					//	throw new Exception("Unsupported hex record type");
				}
			}
			tr.Close();
			fs.Close();
			if(memLength>=flashSize)
				throw new Exception("Program is larger than flash!");
		}
		void RichTextBox1TextChanged(object sender, EventArgs e)
		{
			richTextBox1.ScrollToCaret();
		}
		void Button3Click(object sender, EventArgs e)
		{
			UpdatePortList();
		}
		void RichTextBox1Resize(object sender, EventArgs e)
		{
			richTextBox1.ScrollToCaret();
		}
		void MainFormKeyPress(object sender, KeyPressEventArgs e)
		{
			switch(e.KeyChar)
			{
				case (char)Keys.Escape:
					Application.Exit();
					break;
			}
		}
		void Button4Click(object sender, EventArgs e)
		{
			Ping();
		}
		void Button5Click(object sender, EventArgs e)
		{
			Quit();
		}
		void Quit()
		{
			Application.Exit();
			notifyIcon1.Visible=false;
		}
		void MainFormResize(object sender, EventArgs e)
		{
			if(WindowState==FormWindowState.Minimized)
			{
				Hide();
				notifyIcon1.Visible=true;
				if(notifyIcon1.Tag==null)
				{
					notifyIcon1.Tag="!";
					notifyIcon1.ShowBalloonTip(2);
				}
			}
		}
		void QuitToolStripMenuItemClick(object sender, EventArgs e)
		{
			Quit();
		}
		void PingToolStripMenuItemClick(object sender, EventArgs e)
		{
			if(Ping())
				notifyIcon1.ShowBalloonTip(2,"Ping","Target replied :)",ToolTipIcon.None);
			else
				notifyIcon1.ShowBalloonTip(2,"Ping","Target did NOT reply :(",ToolTipIcon.Error);
		}
		void NotifyIcon1MouseClick(object sender, MouseEventArgs e)
		{
			if(e.Button==MouseButtons.Left)
			{
				Show();
				WindowState= FormWindowState.Normal;
				notifyIcon1.Visible=false;
			}
		}
		void UpdateToolStripMenuItemClick(object sender, EventArgs e)
		{
			if(UpdateTarget())
				notifyIcon1.ShowBalloonTip(2,"Update","Target updated :)",ToolTipIcon.None);
			else
				notifyIcon1.ShowBalloonTip(2,"Update","Could NOT update target :(",ToolTipIcon.Error);			
		}
		void BrowseToolStripMenuItemClick(object sender, EventArgs e)
		{
			Browse();
		}
		void UpdateNotifyIconText()
		{
			string s="Firmware Updater\nPort: "+comboBox1.Text+"\nFile: "+textBox1.Text.Substring(textBox1.Text.LastIndexOf('/')+1);
			if(s.Length<=64)
				notifyIcon1.Text=s;
			else
				notifyIcon1.Text=s.Substring(0,60)+"...";
		}
		void ComboBox1TextUpdate(object sender, EventArgs e)
		{
			UpdateNotifyIconText();
		}
	}
}
