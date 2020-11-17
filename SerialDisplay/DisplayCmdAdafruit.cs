using System;
using System.Runtime.InteropServices;

// ReSharper disable MemberCanBePrivate.Global
// ReSharper disable FieldCanBeMadeReadOnly.Global
// ReSharper disable ConvertToConstant.Global

namespace SerialDisplay
{
  public static unsafe class DisplayCmdAdafruit
  {
    /// <summary>
    /// Display-Width
    /// </summary>
    public const int Width = 320;
    /// <summary>
    /// Display-Height
    /// </summary>
    public const int Height = 240;

    /// <summary>
    /// current display-width
    /// </summary>
    public static int currentWidth = Width;
    /// <summary>
    /// current display-height
    /// </summary>
    public static int currentHeight = Height;
    /// <summary>
    /// current display-rotation
    /// </summary>
    public static int currentRotation = 0;
    /// <summary>
    /// current selected backbuffer (0-255)
    /// </summary>
    public static int currentBackbuffer = 0;
    /// <summary>
    /// full backbuffers
    /// </summary>
    public static readonly uint* backbufferData = (uint*)Marshal.AllocHGlobal(Width * Height * sizeof(uint) * 256);


    /// <summary>
    /// command-types
    /// </summary>
    enum CmdAdafruitType
    {
      /// <summary>
      /// fills the entire screen with one colour
      /// [uint16_t color]
      /// </summary>
      CmdFillScreen = 0x01,
      /// <summary>
      /// fast drawing of a horizontal line
      /// [int16_t x, int16_t y, int16_t w, uint16_t color]
      /// </summary>
      CmdFastHLine = 0x02,
      /// <summary>
      /// fast drawing of a vertical line
      /// [int16_t x, int16_t y, int16_t h, uint16_t color]
      /// </summary>
      CmdFastVLine = 0x03,
      /// <summary>
      /// drawing normal line
      /// [int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color]
      /// </summary>
      CmdDrawLine = 0x04,
      /// <summary>
      /// set display rotation (0-3)
      /// [uint8_t rotation]
      /// </summary>
      CmdSetRotation = 0x05,
      /// <summary>
      /// set backbuffer (0-255)
      /// [uint8_t index]
      /// </summary>
      CmdSetBackBuffer = 0x06,
      /// <summary>
      /// copy current display to another backbuffer (or frontbuffer)
      /// [uint8_t index]
      /// </summary>
      CmdCopyToBackbuffer = 0x07,
    }

    /// <summary>
    /// get the length of the command
    /// </summary>
    /// <param name="cmd">command to check</param>
    /// <returns>length of the command in bytes</returns>
    public static int CommandLength(byte cmd)
    {
      switch ((CmdAdafruitType)cmd)
      {
        case CmdAdafruitType.CmdFillScreen: return 1 + sizeof(ushort);
        case CmdAdafruitType.CmdFastHLine: return 1 + sizeof(ushort) * 4;
        case CmdAdafruitType.CmdFastVLine: return 1 + sizeof(ushort) * 4;
        case CmdAdafruitType.CmdDrawLine: return 1 + sizeof(ushort) * 5;
        case CmdAdafruitType.CmdSetRotation: return 1 + sizeof(byte);
        case CmdAdafruitType.CmdSetBackBuffer: return 1 + sizeof(byte);
        case CmdAdafruitType.CmdCopyToBackbuffer: return 1 + sizeof(byte);
        default: return 1; // unknown command
      }
    }

    /// <summary>
    /// convert 16-Bit BGR-color to 32-Bit ARGB
    /// </summary>
    /// <param name="color">color565</param>
    /// <returns>ARGB-Color</returns>
    static uint Color565ToArgb(ushort color)
    {
      uint c = color;
      return (c & 0xf800) << 8 | (c & 0x07e0) << 5 | (c & 0x001f) << 3 | 0xff000000;
    }

    #region # // --- FillScreen ---
    /// <summary>
    /// fills the entire screen with one colour
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="color">fill-color</param>
    static void FillScreen(uint* ptr, uint color)
    {
      ulong colorLong = color | (ulong)color << 32;
      for (int i = 0; i < Width * Height; i += 8)
      {
        *(ulong*)(ptr + i) = colorLong;
        *(ulong*)(ptr + i + 2) = colorLong;
        *(ulong*)(ptr + i + 4) = colorLong;
        *(ulong*)(ptr + i + 6) = colorLong;
      }
    }
    #endregion

    #region # // --- FastHLine + FastVLine ---
    /// <summary>
    /// draw fast horizontal line
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer (+start position)</param>
    /// <param name="len">length of the line</param>
    /// <param name="color">line-color</param>
    static void FastHLine(uint* ptr, int len, uint color)
    {
      for (int i = 0; i < len; i++) ptr[i] = color;
    }

    /// <summary>
    /// draw fast vertical line
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer (+start position)</param>
    /// <param name="len">length of the line</param>
    /// <param name="color">line-color</param>
    static void FastVLine(uint* ptr, int len, uint color)
    {
      len *= Width;
      for (int i = 0; i < len; i += Width) ptr[i] = color;
    }

    /// <summary>
    /// draw fast horizontal line
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="x">start x-position</param>
    /// <param name="y">start y-position</param>
    /// <param name="w">width / length</param>
    /// <param name="color">line-color</param>
    static void FastHLine(uint* ptr, int x, int y, int w, uint color)
    {
      if (w < 0) // Convert negative widths to positive equivalent
      {
        w *= -1;
        x -= w - 1;
        if (x < 0)
        {
          w += x;
          x = 0;
        }
      }

      if ((uint)y >= currentHeight || x >= currentWidth || x + w - 1 < 0) // Edge rejection (no-draw if totally off canvas)
      {
        return;
      }

      if (x < 0) // Clip left
      {
        w += x;
        x = 0;
      }
      if (x + w >= currentWidth) // Clip right
      {
        w = currentWidth - x;
      }

      if (currentRotation == 0)
      {
        FastHLine(ptr + x + y * Width, w, color);
      }
      else if (currentRotation == 1)
      {
        int t = x;
        x = Width - 1 - y;
        y = t;
        FastVLine(ptr + x + y * Width, w, color);
      }
      else if (currentRotation == 2)
      {
        x = Width - 1 - x;
        y = Height - 1 - y;
        x -= w - 1;
        FastHLine(ptr + x + y * Width, w, color);
      }
      else
      {
        int t = x;
        x = y;
        y = Height - 1 - t;
        y -= w - 1;
        FastVLine(ptr + x + y * Width, w, color);
      }
    }

    /// <summary>
    /// draw fast vertical line
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="x">start x-position</param>
    /// <param name="y">start y-position</param>
    /// <param name="h">height / length</param>
    /// <param name="color">line-color</param>
    static void FastVLine(uint* ptr, int x, int y, int h, uint color)
    {
      if (h < 0) // Convert negative heights to positive equivalent
      {
        h *= -1;
        y -= h - 1;
        if (y < 0)
        {
          h += y;
          y = 0;
        }
      }

      // Edge rejection (no-draw if totally off canvas)
      if ((uint)x >= currentWidth || y >= currentHeight || y + h - 1 < 0) return;

      if (y < 0) // Clip top
      {
        h += y;
        y = 0;
      }
      if (y + h > currentHeight) // Clip bottom
      {
        h = currentHeight - y;
      }

      if (currentRotation == 0)
      {
        FastVLine(ptr + x + y * Width, h, color);
      }
      else if (currentRotation == 1)
      {
        int t = x;
        x = Width - 1 - y;
        y = t;
        x -= h - 1;
        FastHLine(ptr + x + y * Width, h, color);
      }
      else if (currentRotation == 2)
      {
        x = Width - 1 - x;
        y = Height - 1 - y;
        y -= h - 1;
        FastVLine(ptr + x + y * Width, h, color);
      }
      else
      {
        int t = x;
        x = y;
        y = Height - 1 - t;
        FastHLine(ptr + x + y * Width, h, color);
      }
    }
    #endregion

    #region # // --- DrawLine ---
    /// <summary>
    /// drawing normal line (safe-mode)
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer (+start position)</param>
    /// <param name="x1">start x-position</param>
    /// <param name="y1">start y-position</param>
    /// <param name="x2">end x-position</param>
    /// <param name="y2">end y-position</param>
    /// <param name="color">line-color</param>
    static void DrawLineSafe(uint* ptr, int x1, int y1, int x2, int y2, uint color)
    {
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
          if ((uint)y1 < Width && (uint)x1 < Height) ptr[y1 + x1 * Width] = color;
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
          if ((uint)x1 < Width && (uint)y1 < Height) ptr[x1 + y1 * Width] = color;
          err -= dy;
          if (err < 0)
          {
            y1 += ystep;
            err += dx;
          }
        }
      }
    }

    /// <summary>
    /// drawing normal line
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer (+start position)</param>
    /// <param name="x1">start x-position</param>
    /// <param name="y1">start y-position</param>
    /// <param name="x2">end x-position</param>
    /// <param name="y2">end y-position</param>
    /// <param name="color">line-color</param>
    static void DrawLineFast(uint* ptr, int x1, int y1, int x2, int y2, uint color)
    {
      if ((uint)x1 < Width && (uint)y1 < Height && (uint)x2 < Width && (uint)y2 < Height)
      {
        bool steep = Math.Abs(y2 - y1) > Math.Abs(x2 - x1);
        if (steep) { int t = x1; x1 = y1; y1 = t; t = x2; x2 = y2; y2 = t; }
        if (x1 > x2) { int t = x1; x1 = x2; x2 = t; t = y1; y1 = y2; y2 = t; }

        int dx = x2 - x1;
        int dy = Math.Abs(y2 - y1);
        int err = dx / 2;
        int ystep = y1 < y2 ? 1 : -1;

        if (steep)
        {
          var p = ptr + y1 + x1 * Width;
          for (; x1 <= x2; x1++)
          {
            *p = color;
            err -= dy;
            p += Width;
            if (err < 0)
            {
              p += ystep;
              err += dx;
            }
          }
        }
        else
        {
          var p = ptr + x1 + y1 * Width;
          ystep *= Width;
          for (; x1 <= x2; x1++)
          {
            *p = color;
            err -= dy;
            p++;
            if (err < 0)
            {
              err += dx;
              p += ystep;
            }
          }
        }
      }
      else
      {
        DrawLineSafe(ptr, x1, y1, x2, y2, color);
      }
    }

    /// <summary>
    /// drawing normal line
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer (+start position)</param>
    /// <param name="x1">start x-position</param>
    /// <param name="y1">start y-position</param>
    /// <param name="x2">end x-position</param>
    /// <param name="y2">end y-position</param>
    /// <param name="color">line-color</param>
    static void DrawLine(uint* ptr, int x1, int y1, int x2, int y2, uint color)
    {
      if (currentRotation == 0)
      {
        DrawLineFast(ptr, x1, y1, x2, y2, color);
      }
      else if (currentRotation == 1)
      {
        DrawLineFast(ptr, Width - 1 - y1, x1, Width - 1 - y2, x2, color);
      }
      else if (currentRotation == 2)
      {
        DrawLineFast(ptr, Width - 1 - x1, Height - 1 - y1, Width - 1 - x2, Height - 1 - y2, color);
      }
      else
      {
        DrawLineFast(ptr, y1, Height - 1 - x1, y2, Height - 1 - x2, color);
      }
    }
    #endregion

    #region # --- CopyToBackbuffer ---
    /// <summary>
    /// copy entire backbuffer
    /// </summary>
    /// <param name="from">From-Backbuffer</param>
    /// <param name="to">To-Backbuffer</param>
    static void CopyToBackbuffer(uint* from, uint* to)
    {
      for (int i = 0; i < Width * Height; i += 8)
      {
        ulong tmp0 = *(ulong*)(from + i);
        ulong tmp1 = *(ulong*)(from + i+2);
        ulong tmp2 = *(ulong*)(from + i+4);
        ulong tmp3 = *(ulong*)(from + i+6);
        *(ulong*)(to + i) = tmp0;
        *(ulong*)(to + i + 2) = tmp1;
        *(ulong*)(to + i + 4) = tmp2;
        *(ulong*)(to + i + 6) = tmp3;
      }
    }
    #endregion

    /// <summary>
    /// executes a drawing command
    /// </summary>
    /// <param name="buffer">buffer with commands</param>
    /// <param name="bufferOfs">offset inner buffer</param>
    /// <param name="bitmapPtr">pointer to output-bitmap</param>
    /// <returns>read command bytes</returns>
    public static int Execute(byte[] buffer, int bufferOfs, uint* bitmapPtr)
    {
      byte cmd = buffer[bufferOfs++];
      var p = currentBackbuffer != 0 ? &backbufferData[Width * Height * sizeof(uint) * currentBackbuffer] : bitmapPtr;

      switch ((CmdAdafruitType)cmd)
      {
        case CmdAdafruitType.CmdFillScreen:
        {
          ushort color = BitConverter.ToUInt16(buffer, bufferOfs);
          FillScreen(p, Color565ToArgb(color));
          return 1 + sizeof(ushort);
        }

        case CmdAdafruitType.CmdFastHLine:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int w = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          ushort color = BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 3);
          FastHLine(p, x, y, w, Color565ToArgb(color));
          return 1 + sizeof(ushort) * 4;
        }

        case CmdAdafruitType.CmdFastVLine:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int h = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          ushort color = BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 3);
          FastVLine(p, x, y, h, Color565ToArgb(color));
          return 1 + sizeof(ushort) * 4;
        }

        case CmdAdafruitType.CmdDrawLine:
        {
          int x1 = BitConverter.ToInt16(buffer, bufferOfs);
          int y1 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int x2 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          int y2 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 3);
          ushort color = BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 4);
          DrawLine(p, x1, y1, x2, y2, Color565ToArgb(color));
          return 1 + sizeof(ushort) * 5;
        }

        case CmdAdafruitType.CmdSetRotation:
        {
          currentRotation = buffer[bufferOfs] & 3;
          if ((currentRotation & 1) == 0)
          {
            currentWidth = Width;
            currentHeight = Height;
          }
          else
          {
            currentWidth = Height;
            currentHeight = Width;
          }
          return 1 + sizeof(byte);
        }

        case CmdAdafruitType.CmdSetBackBuffer:
        {
          currentBackbuffer = buffer[bufferOfs];
          return 1 + sizeof(byte);
        }

        case CmdAdafruitType.CmdCopyToBackbuffer:
        {
          uint index = buffer[bufferOfs];
          CopyToBackbuffer(p, index != 0 ? &backbufferData[Width * Height * sizeof(uint) * index] : bitmapPtr);
          return 1 + sizeof(byte);
        }

        default: return 1; // unknown command
      }
    }
  }
}
