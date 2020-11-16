using System;
using System.IO.Ports;
using System.Windows.Forms;

namespace SerialDisplay
{
  public partial class DisplayForm : Form
  {
    readonly SerialPort serialPort = new SerialPort("COM7", 2000000);
    int totalBytes;

    public DisplayForm()
    {
      InitializeComponent();
    }

    void displayTimer_Tick(object sender, EventArgs e)
    {
      if (!serialPort.IsOpen)
      {
        try
        {
          serialPort.Open();
        }
        catch { }
        return;
      }
      int count = serialPort.BytesToRead;
      if (count == 0) return;
      var buf = new byte[count];
      int c = serialPort.Read(buf, 0, count);
      totalBytes += c;
      Text = totalBytes.ToString("N0");
    }
  }
}
