using System;
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
    /// command-types
    /// </summary>
    enum CmdAdafruitType
    {
      /// <summary>
      /// fills the entire screen with one colour [uint16_t color]
      /// </summary>
      CmdFillScreen = 0x01,
      /// <summary>
      /// fast drawing of a horizontal line [int16_t x, int16_t y, int16_t w, uint16_t color]
      /// </summary>
      CmdFastHLine = 0x02,
      /// <summary>
      /// fast drawing of a vertical line [int16_t x, int16_t y, int16_t h, uint16_t color]
      /// </summary>
      CmdFastVLine = 0x03,
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
      if (y + h >currentHeight) // Clip bottom
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
      switch ((CmdAdafruitType)cmd)
      {
        case CmdAdafruitType.CmdFillScreen:
        {
          ushort color = BitConverter.ToUInt16(buffer, bufferOfs);
          FillScreen(bitmapPtr, Color565ToArgb(color));
          return 1 + sizeof(ushort);
        }

        case CmdAdafruitType.CmdFastHLine:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int w = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          ushort color = BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 3);
          FastHLine(bitmapPtr, x, y, w, Color565ToArgb(color));
          return 1 + sizeof(ushort) * 4;
        }

        case CmdAdafruitType.CmdFastVLine:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int h = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          ushort color = BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 3);
          FastVLine(bitmapPtr, x, y, h, Color565ToArgb(color));
          return 1 + sizeof(ushort) * 4;
        }

        default: return 1; // unknown command
      }
    }
  }
}
