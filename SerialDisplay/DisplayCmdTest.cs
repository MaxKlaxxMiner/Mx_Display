
using System;

namespace SerialDisplay
{
  public static unsafe class DisplayCmdTest
  {
    public const int Width = 320;
    public const int Height = 240;
    static uint currentColor = 0xffffff;

    public static int CommandLength(byte type)
    {
      switch ((DisplayCmdTestType)type)
      {
        case DisplayCmdTestType.SelectColor565: return 1 + sizeof(ushort);
        case DisplayCmdTestType.SelectColorTrue: return 1 + sizeof(byte) * 3;
        case DisplayCmdTestType.DrawPixel: return 1 + sizeof(short) + sizeof(short);
        case DisplayCmdTestType.DrawHLine: return 1 + sizeof(short) + sizeof(short) + sizeof(short);
        case DisplayCmdTestType.DrawVLine: return 1 + sizeof(short) + sizeof(short) + sizeof(short);
        case DisplayCmdTestType.DrawLine: return 1 + sizeof(short) + sizeof(short) + sizeof(short) + sizeof(short);
        default: return 1;
      }
    }

    public static int Execute(byte[] buffer, int bufferReadPos, uint* bitmapPtr)
    {
      switch ((DisplayCmdTestType)buffer[bufferReadPos++])
      {
        case DisplayCmdTestType.GetInfo: return 1;
        case DisplayCmdTestType.SelectColor565:
        {
          var c = (uint)BitConverter.ToUInt16(buffer, bufferReadPos);
          currentColor = (c & 0xf800) << 8 | (c & 0x07e0) << 5 | (c & 0x001f) << 3;
          return 1 + sizeof(ushort);
        }
        case DisplayCmdTestType.SelectColorTrue:
        {
          uint c = buffer[bufferReadPos++];
          c |= (uint)buffer[bufferReadPos++] << 8;
          c |= (uint)buffer[bufferReadPos] << 16;
          currentColor = c;
          return 1 + sizeof(byte) * 3;
        }
        case DisplayCmdTestType.DrawPixel:
        {
          short x = BitConverter.ToInt16(buffer, bufferReadPos);
          short y = BitConverter.ToInt16(buffer, bufferReadPos + sizeof(short));
          if ((ushort)x < Width && (ushort)y < Height)
          {
            bitmapPtr[x + y * Width] = currentColor;
          }
          return 1 + sizeof(short) + sizeof(short);
        }
        case DisplayCmdTestType.DrawHLine:
        {
          int x = BitConverter.ToInt16(buffer, bufferReadPos);
          int y = BitConverter.ToInt16(buffer, bufferReadPos + sizeof(short));
          int w = BitConverter.ToInt16(buffer, bufferReadPos + sizeof(short) + sizeof(short));

          if ((uint)y < Height && x < Width)
          {
            if (x < 0)
            {
              w += x;
              x = 0;
            }
            if (x + w > Width) w = Width - x;
            if ((uint)x <= Width && w > 0)
            {
              var p = &bitmapPtr[x + y * Width];
              for (int i = 0; i < w; i++) p[i] = currentColor;
            }
          }
          return 1 + sizeof(short) + sizeof(short) + sizeof(short);
        }
        case DisplayCmdTestType.DrawVLine:
        {
          int x = BitConverter.ToInt16(buffer, bufferReadPos);
          int y = BitConverter.ToInt16(buffer, bufferReadPos + sizeof(short));
          int h = BitConverter.ToInt16(buffer, bufferReadPos + sizeof(short) + sizeof(short));

          if (y < Height && (uint)x < Width)
          {
            if (y < 0)
            {
              h += y;
              y = 0;
            }
            if (y + h > Height) h = Height - y;
            if ((uint)y <= Height && h > 0)
            {
              var p = &bitmapPtr[x + y * Width];
              for (int i = 0; i < h; i++) p[i * Width] = currentColor;
            }
          }
          return 1 + sizeof(short) + sizeof(short) + sizeof(short);
        }
        case DisplayCmdTestType.DrawLine:
        {
          int x1 = BitConverter.ToInt16(buffer, bufferReadPos);
          int y1 = BitConverter.ToInt16(buffer, bufferReadPos + sizeof(short));
          int x2 = BitConverter.ToInt16(buffer, bufferReadPos + sizeof(short) + sizeof(short));
          int y2 = BitConverter.ToInt16(buffer, bufferReadPos + sizeof(short) + sizeof(short) + sizeof(short));

          bool steep = Math.Abs(y2 - y1) > Math.Abs(x2 - x1);
          if (steep) { int t = x1; x1 = y1; y1 = t; t = x2; x2 = y2; y2 = t; }
          if (x1 > x2) { int t = x1; x1 = x2; x2 = t; t = y1; y1 = y2; y2 = t; }

          int dx = x2 - x1;
          int dy = Math.Abs(y2 - y1);
          int err = dx / 2;
          int ystep = y1 < y2 ? 1 : -1;

          if (steep)
          {
            for (; x1 <= x2; x1++)
            {
              if ((uint)y1 < Width && (uint)x1 < Height) bitmapPtr[y1 + x1 * Width] = currentColor;
              err -= dy;
              if (err < 0)
              {
                y1 += ystep;
                err += dx;
              }
            }
          }
          else
          {
            for (; x1 <= x2; x1++)
            {
              if ((uint)x1 < Width && (uint)y1 < Height) bitmapPtr[x1 + y1 * Width] = currentColor;
              err -= dy;
              if (err < 0)
              {
                y1 += ystep;
                err += dx;
              }
            }
          }

          return 1 + sizeof(short) + sizeof(short) + sizeof(short) + sizeof(short);
        }
        default: return 1; // unknown Command
      }
    }
  }
}
