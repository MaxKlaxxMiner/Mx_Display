using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;
using DisplayCmd = SerialDisplay.DisplayCmdAdafruit;

namespace SerialDisplay
{
  public unsafe sealed partial class DisplayForm : Form
  {
    const string DisplayPort = "COM3";

    readonly SerialPort serialPort = new SerialPort(DisplayPort, 500000);
    static readonly Queue<byte[]> buffers = new Queue<byte[]>();
    static readonly byte[] buffer = new byte[65536];
    static int bufferReadPos;
    static int bufferWritePos;
    const int MaxCmdLength = 1 + sizeof(short) * 7;

    public DisplayForm()
    {
      InitializeComponent();
      var mainThread = Thread.CurrentThread;
      Text = "Display: " + DisplayCmd.Width + " x " + DisplayCmd.Height + " @" + DisplayPort;
      new Thread(() =>
      {
        while (mainThread.IsAlive)
        {
          if (!serialPort.IsOpen)
          {
            try
            {
              serialPort.Open();
            }
            catch { }
            Thread.Sleep(10);
            continue;
          }
          int count = Math.Min(serialPort.BytesToRead, 4096);
          if (count == 0) continue;
          var buf = new byte[count];
          int c = serialPort.Read(buf, 0, count);
          if (c == count)
          {
            lock (buffers)
            {
              buffers.Enqueue(buf);
            }
          }
          Thread.Sleep(0);
        }
      }).Start();
    }

    Bitmap bitmap;

    static DisplayForm()
    {
      bufferReadPos = 0;
    }

    void TestImage()
    {
      var bitmapData = bitmap.LockBits(new Rectangle(0, 0, DisplayCmd.Width, DisplayCmd.Height), ImageLockMode.WriteOnly, PixelFormat.Format32bppRgb);
      var bitmapPtr = (uint*)bitmapData.Scan0;

      for (uint y = 0; y < DisplayCmd.Height; y++)
      {
        for (uint x = 0; x < DisplayCmd.Width; x++)
        {
          uint c = x * 255 / DisplayCmd.Width;
          if (y < DisplayCmd.Height / 4) bitmapPtr[x + y * DisplayCmd.Width] = c << 16;
          else if (y < DisplayCmd.Height / 4 * 2) bitmapPtr[x + y * DisplayCmd.Width] = c << 8;
          else if (y < DisplayCmd.Height / 4 * 3) bitmapPtr[x + y * DisplayCmd.Width] = c;
          else bitmapPtr[x + y * DisplayCmd.Width] = c | c << 8 | c << 16;
        }
      }

      bitmap.UnlockBits(bitmapData);
    }

    static bool CommandAvail()
    {
      int filled = bufferWritePos - bufferReadPos;
      if (filled >= MaxCmdLength) return true;

      lock (buffers)
      {
        if (buffers.Count > 0)
        {
          var newBuffer = buffers.Dequeue();
          if (newBuffer.Length > buffer.Length - bufferWritePos)
          {
            Array.Copy(buffer, bufferReadPos, buffer, 0, filled);
            bufferWritePos -= bufferReadPos;
            bufferReadPos = 0;
          }
          Array.Copy(newBuffer, 0, buffer, bufferWritePos, newBuffer.Length);
          bufferWritePos += newBuffer.Length;
          return CommandAvail();
        }
      }

      return filled >= DisplayCmd.CommandLength(buffer[bufferReadPos]);
    }

    void displayTimer_Tick(object sender, EventArgs e)
    {
      if (bitmap == null)
      {
        bitmap = new Bitmap(DisplayCmd.Width, DisplayCmd.Height, PixelFormat.Format32bppRgb);
        BackgroundImage = bitmap;
        TestImage();
      }

      if (!CommandAvail()) return;

      var bitmapData = bitmap.LockBits(new Rectangle(0, 0, DisplayCmd.Width, DisplayCmd.Height), ImageLockMode.WriteOnly, PixelFormat.Format32bppRgb);
      var bitmapPtr = (uint*)bitmapData.Scan0;

      while (CommandAvail())
      {
        bufferReadPos += DisplayCmd.Execute(buffer, bufferReadPos, bitmapPtr);
      }

      bitmap.UnlockBits(bitmapData);
      Invalidate();
    }

    protected override void OnPaintBackground(PaintEventArgs pevent)
    {
      if (BackgroundImage != null)
      {
        pevent.Graphics.InterpolationMode = InterpolationMode.NearestNeighbor;
        //pevent.Graphics.Clear(BackColor);
        pevent.Graphics.DrawImage(BackgroundImage, 0, 0, ClientSize.Width, ClientSize.Height);
      }
      else
      {
        base.OnPaintBackground(pevent);
      }
    }

    private void DisplayForm_Load(object sender, EventArgs e)
    {
      int sizeX = DisplayCmd.Width;
      int sizeY = DisplayCmd.Height;
      var screen = Screen.PrimaryScreen.Bounds.Size;
      while (sizeX + DisplayCmd.Width < screen.Width - 32 && sizeY + DisplayCmd.Height < screen.Height - 32)
      {
        sizeX += DisplayCmd.Width;
        sizeY += DisplayCmd.Height;
      }
      SetClientSizeCore(sizeX, sizeY);
      Top = 0;
    }

    private void DisplayForm_Resize(object sender, EventArgs e)
    {
      Refresh();
    }
  }
}
