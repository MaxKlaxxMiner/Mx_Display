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
    public static int currentRotation;
    /// <summary>
    /// current selected backbuffer (0-255)
    /// </summary>
    public static int currentBackbuffer;
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
      /// empty command (no operation)
      /// </summary>
      CmdNop = 0x00,
      /// <summary>
      /// fills the entire screen with one colour
      /// [uint16_t color]
      /// </summary>
      CmdFillScreen,
      /// <summary>
      /// draw a pixel :)
      /// [int16_t x, int16_t y, uint16_t color]
      /// </summary>
      CmdDrawPixel,
      /// <summary>
      /// fast drawing of a horizontal line
      /// [int16_t x, int16_t y, int16_t w, uint16_t color]
      /// </summary>
      CmdFastHLine,
      /// <summary>
      /// fast drawing of a vertical line
      /// [int16_t x, int16_t y, int16_t h, uint16_t color]
      /// </summary>
      CmdFastVLine,
      /// <summary>
      /// drawing normal line
      /// [int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color]
      /// </summary>
      CmdDrawLine,
      /// <summary>
      /// draw a rectangle with no fill color
      /// [int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color]
      /// </summary>
      CmdDrawRect,
      /// <summary>
      /// fill a rectangle completely with one color
      /// [int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color]
      /// </summary>
      CmdFillRect,
      /// <summary>
      /// draw a circle outline
      /// [int16_t x, int16_t y, int16_t r, uint16_t color]
      /// </summary>
      CmdDrawCircle,
      /// <summary>
      /// draw a circle with filled color
      /// [int16_t x, int16_t y, int16_t r, uint16_t color]
      /// </summary>
      CmdFillCircle,
      /// <summary>
      /// draw a rounded rectangle with no fill color
      /// [int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color]
      /// </summary>
      CmdDrawRoundRect,
      /// <summary>
      /// draw a rounded rectangle with fill color
      /// [int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color]
      /// </summary>
      CmdFillRoundRect,
      /// <summary>
      /// draw a triangle with no fill color
      /// [int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color]
      /// </summary>
      CmdDrawTriangle,
      /// <summary>
      /// draw a triangle with filled color
      /// [int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color]
      /// </summary>
      CmdFillTriangle,
      /// <summary>
      /// set text cursor location
      /// [int16_t x, int16_t y]
      /// </summary>
      CmdSetCursor,
      /// <summary>
      /// set text font color and background color (color = bgColor: transparent background)
      /// [uint16_t color, uint16_t bgColor]
      /// </summary>
      CmdSetTextColor,
      /// <summary>
      /// set text size
      /// [uint8_t sizeX, uint8_t sizeY]
      /// </summary>
      CmdSetTextSize,
      /// <summary>
      /// set whether text that is too long for the screen width should automatically wrap around to the next line (else clip right).
      /// [bool wrap]
      /// </summary>
      CmdSetTextWrap,
      /// <summary>
      /// write a character
      /// [uint8_t character]
      /// </summary>
      CmdWriteChar,
      /// <summary>
      /// set display rotation (0-3)
      /// [uint8_t rotation]
      /// </summary>
      CmdSetRotation,
      /// <summary>
      /// set backbuffer (0-255)
      /// [uint8_t index]
      /// </summary>
      CmdSetBackBuffer,
      /// <summary>
      /// copy current display to another backbuffer (or frontbuffer)
      /// [uint8_t index]
      /// </summary>
      CmdCopyToBackbuffer,
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
        case CmdAdafruitType.CmdNop: return 1;
        case CmdAdafruitType.CmdFillScreen: return 1 + sizeof(ushort);
        case CmdAdafruitType.CmdDrawPixel: return 1 + sizeof(ushort) * 3;
        case CmdAdafruitType.CmdFastHLine:
        case CmdAdafruitType.CmdFastVLine: return 1 + sizeof(ushort) * 4;
        case CmdAdafruitType.CmdDrawLine:
        case CmdAdafruitType.CmdDrawRect:
        case CmdAdafruitType.CmdFillRect: return 1 + sizeof(ushort) * 5;
        case CmdAdafruitType.CmdDrawCircle:
        case CmdAdafruitType.CmdFillCircle: return 1 + sizeof(ushort) * 4;
        case CmdAdafruitType.CmdDrawRoundRect:
        case CmdAdafruitType.CmdFillRoundRect: return 1 + sizeof(ushort) * 6;
        case CmdAdafruitType.CmdDrawTriangle:
        case CmdAdafruitType.CmdFillTriangle: return 1 + sizeof(ushort) * 7;
        case CmdAdafruitType.CmdSetCursor:
        case CmdAdafruitType.CmdSetTextColor: return 1 + sizeof(ushort) * 2;
        case CmdAdafruitType.CmdSetTextSize: return 1 + sizeof(byte) * 2;
        case CmdAdafruitType.CmdSetTextWrap:
        case CmdAdafruitType.CmdWriteChar: return 1 + sizeof(byte);
        case CmdAdafruitType.CmdSetRotation:
        case CmdAdafruitType.CmdSetBackBuffer:
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

    #region # // --- FillScreen + DrawPixel ---
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

    /// <summary>
    /// draw a pixel :)
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="x">x-position</param>
    /// <param name="y">y-position</param>
    /// <param name="color">pixel-color</param>
    static void DrawPixel(uint* ptr, int x, int y, uint color)
    {
      if ((uint)x >= currentWidth || (uint)y >= currentHeight) return;

      if (currentRotation == 0)
      {
        ptr[x + y * Width] = color;
      }
      else if (currentRotation == 1)
      {
        ptr[Width - 1 - y + x * Width] = color;
      }
      else if (currentRotation == 2)
      {
        ptr[Width - 1 - x + (Height - 1 - y) * Width] = color;
      }
      else
      {
        ptr[y + (Height - 1 - x) * Width] = color;
      }
    }
    #endregion

    #region # // --- FastHLine + FastVLine + FillRect ---
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

      // Edge rejection (no-draw if totally off canvas)
      if ((uint)y >= currentHeight || x >= currentWidth || x + w - 1 < 0)
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

    /// <summary>
    /// draw a filled rect
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="x">start x-position</param>
    /// <param name="y">start y-position</param>
    /// <param name="w">width</param>
    /// <param name="h">height</param>
    /// <param name="color">fill-color</param>
    static void FillRect(uint* ptr, int x, int y, int w, int h, uint color)
    {
      for (int i = 0; i < h; i++) FastHLine(ptr, x, y + i, w, color);
    }
    #endregion

    #region # // --- DrawLine ---
    /// <summary>
    /// drawing normal line (safe-mode)
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
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
    /// <param name="ptr">pointer to the backbuffer</param>
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

    #region # // --- CopyToBackbuffer ---
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
        ulong tmp1 = *(ulong*)(from + i + 2);
        ulong tmp2 = *(ulong*)(from + i + 4);
        ulong tmp3 = *(ulong*)(from + i + 6);
        *(ulong*)(to + i) = tmp0;
        *(ulong*)(to + i + 2) = tmp1;
        *(ulong*)(to + i + 4) = tmp2;
        *(ulong*)(to + i + 6) = tmp3;
      }
    }
    #endregion

    #region # // --- DrawCircle ---
    /// <summary>
    /// draw a circle outline
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="x">center x-position</param>
    /// <param name="y">center y-position</param>
    /// <param name="r">radius</param>
    /// <param name="color">border-color</param>
    static void DrawCircle(uint* ptr, int x, int y, int r, uint color)
    {
      if (currentRotation != 0)
      {
        if (currentRotation == 1)
        {
          int t = x;
          x = Width - 1 - y;
          y = t;
        }
        else if (currentRotation == 2)
        {
          x = Width - 1 - x;
          y = Height - 1 - y;
        }
        else
        {
          int t = x;
          x = y;
          y = Height - 1 - t;
        }
      }

      int f = 1 - r;
      int ddF_x = 1;
      int ddF_y = -2 * r;
      int px = 0;
      int py = r;

      if ((uint)x < Width)
      {
        if ((uint)(y + r) < Height) ptr[x + (y + r) * Width] = color;
        if ((uint)(y - r) < Height) ptr[x + (y - r) * Width] = color;
      }
      if ((uint)y < Height)
      {
        if ((uint)(x + r) < Width) ptr[x + r + y * Width] = color;
        if ((uint)(x - r) < Width) ptr[x - r + y * Width] = color;
      }

      while (px < py)
      {
        if (f >= 0)
        {
          py--;
          ddF_y += 2;
          f += ddF_y;
        }
        px++;
        ddF_x += 2;
        f += ddF_x;

        if ((uint)(x + px) < Width && (uint)(y + py) < Height) ptr[x + px + (y + py) * Width] = color;
        if ((uint)(x - px) < Width && (uint)(y + py) < Height) ptr[x - px + (y + py) * Width] = color;
        if ((uint)(x + px) < Width && (uint)(y - py) < Height) ptr[x + px + (y - py) * Width] = color;
        if ((uint)(x - px) < Width && (uint)(y - py) < Height) ptr[x - px + (y - py) * Width] = color;
        if ((uint)(x + py) < Width && (uint)(y + px) < Height) ptr[x + py + (y + px) * Width] = color;
        if ((uint)(x - py) < Width && (uint)(y + px) < Height) ptr[x - py + (y + px) * Width] = color;
        if ((uint)(x + py) < Width && (uint)(y - px) < Height) ptr[x + py + (y - px) * Width] = color;
        if ((uint)(x - py) < Width && (uint)(y - px) < Height) ptr[x - py + (y - px) * Width] = color;
      }
    }

    /// <summary>
    /// quarter-circle drawer, used to do circles and roundrects
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="x">center x-position</param>
    /// <param name="y">center y-position</param>
    /// <param name="r">radius</param>
    /// <param name="corners">cornername mask to indicate which quarters of the circle we're doing</param>
    /// <param name="color">border-color</param>
    static void DrawCircleHelper(uint* ptr, int x, int y, int r, int corners, uint color)
    {
      if (currentRotation != 0)
      {
        if (currentRotation == 1)
        {
          int t = x;
          x = Width - 1 - y;
          y = t;
          corners <<= 1;
          corners |= corners >> 4;
        }
        else if (currentRotation == 2)
        {
          x = Width - 1 - x;
          y = Height - 1 - y;
          corners <<= 2;
          corners |= corners >> 4;
        }
        else
        {
          int t = x;
          x = y;
          y = Height - 1 - t;
          corners <<= 3;
          corners |= corners >> 4;
        }
      }

      int f = 1 - r;
      int ddF_x = 1;
      int ddF_y = -2 * r;
      int px = 0;
      int py = r;

      while (px < py)
      {
        if (f >= 0)
        {
          py--;
          ddF_y += 2;
          f += ddF_y;
        }
        px++;
        ddF_x += 2;
        f += ddF_x;
        if ((corners & 0x4) != 0)
        {
          if ((uint)(x + px) < Width && (uint)(y + py) < Height) ptr[x + px + (y + py) * Width] = color;
          if ((uint)(x + py) < Width && (uint)(y + px) < Height) ptr[x + py + (y + px) * Width] = color;
        }
        if ((corners & 0x2) != 0)
        {
          if ((uint)(x + px) < Width && (uint)(y - py) < Height) ptr[x + px + (y - py) * Width] = color;
          if ((uint)(x + py) < Width && (uint)(y - px) < Height) ptr[x + py + (y - px) * Width] = color;
        }
        if ((corners & 0x8) != 0)
        {
          if ((uint)(x - py) < Width && (uint)(y + px) < Height) ptr[x - py + (y + px) * Width] = color;
          if ((uint)(x - px) < Width && (uint)(y + py) < Height) ptr[x - px + (y + py) * Width] = color;
        }
        if ((corners & 0x1) != 0)
        {
          if ((uint)(x - py) < Width && (uint)(y - px) < Height) ptr[x - py + (y - px) * Width] = color;
          if ((uint)(x - px) < Width && (uint)(y - py) < Height) ptr[x - px + (y - py) * Width] = color;
        }
      }
    }

    /// <summary>
    /// quarter-circle drawer with fill, used for circles and roundrects
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="x">center x-position</param>
    /// <param name="y">center y-position</param>
    /// <param name="r">radius</param>
    /// <param name="corners">cornername mask bit #1 or bit #2 to indicate which quarters of the circle we're doing</param>
    /// <param name="delta">offset from center-point, used for round-rects</param>
    /// <param name="color">fill-color</param>
    static void FillCircleHelper(uint* ptr, int x, int y, int r, int corners, int delta, uint color)
    {
      int f = 1 - r;
      int ddF_x = 1;
      int ddF_y = -2 * r;
      int cx = 0;
      int cy = r;
      int px = cx;
      int py = cy;

      delta++; // Avoid some +1's in the loop

      while (cx < cy)
      {
        if (f >= 0)
        {
          cy--;
          ddF_y += 2;
          f += ddF_y;
        }
        cx++;
        ddF_x += 2;
        f += ddF_x;
        if (cx < (cy + 1))
        {
          if ((corners & 1) != 0) FastVLine(ptr, x + cx, y - cy, 2 * cy + delta, color);
          if ((corners & 2) != 0) FastVLine(ptr, x - cx, y - cy, 2 * cy + delta, color);
        }
        if (cy != py)
        {
          if ((corners & 1) != 0) FastVLine(ptr, x + py, y - px, 2 * px + delta, color);
          if ((corners & 2) != 0) FastVLine(ptr, x - py, y - px, 2 * px + delta, color);
          py = cy;
        }
        px = cx;
      }
    }

    /// <summary>
    /// draw a circle with filled color
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="x">center x-position</param>
    /// <param name="y">center y-position</param>
    /// <param name="r">radius</param>
    /// <param name="color">fill-color</param>
    static void FillCircle(uint* ptr, int x, int y, int r, uint color)
    {
      FastVLine(ptr, x, y - r, 2 * r + 1, color);
      FillCircleHelper(ptr, x, y, r, 3, 0, color);
    }
    #endregion

    #region # // --- FillTriangle ---
    static void FillTriangle(uint* ptr, int x0, int y0, int x1, int y1, int x2, int y2, uint color)
    {
      int a, b, y, last;

      // Sort coordinates by Y order (y2 >= y1 >= y0)
      if (y0 > y1)
      {
        int t = y0; y0 = y1; y1 = t;
        t = x0; x0 = x1; x1 = t;
      }
      if (y1 > y2)
      {
        int t = y2; y2 = y1; y1 = t;
        t = x2; x2 = x1; x1 = t;
      }
      if (y0 > y1)
      {
        int t = y0; y0 = y1; y1 = t;
        t = x0; x0 = x1; x1 = t;
      }

      // Handle awkward all-on-same-line case as its own thing
      if (y0 == y2)
      {
        a = b = x0;
        if (x1 < a) a = x1;
        else if (x1 > b) b = x1;
        if (x2 < a) a = x2;
        else if (x2 > b) b = x2;
        FastHLine(ptr, a, y0, b - a + 1, color);
        return;
      }

      int dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0, dx12 = x2 - x1, dy12 = y2 - y1;
      int sa = 0, sb = 0;

      // For upper part of triangle, find scanline crossings for segments
      // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
      // is included here (and second loop will be skipped, avoiding a /0
      // error there), otherwise scanline y1 is skipped here and handled
      // in the second loop...which also avoids a /0 error here if y0=y1
      // (flat-topped triangle).
      if (y1 == y2) last = y1; // Include y1 scanline
      else last = y1 - 1; // Skip it

      for (y = y0; y <= last; y++)
      {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
        {
          int t = a; a = b; b = t;
        }
        FastHLine(ptr, a, y, b - a + 1, color);
      }

      // For lower part of triangle, find scanline crossings for segments
      // 0-2 and 1-2.  This loop is skipped if y1=y2.
      sa = dx12 * (y - y1);
      sb = dx02 * (y - y0);
      for (; y <= y2; y++)
      {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
        {
          int t = a; a = b; b = t;
        }
        FastHLine(ptr, a, y, b - a + 1, color);
      }
    }
    #endregion

    #region # // --- Text ---
    /// <summary>
    /// text-cursor x-position in pixels
    /// </summary>
    static int textCursorX;
    /// <summary>
    /// text-cursor y-position in pixels
    /// </summary>
    static int textCursorY;
    /// <summary>
    /// if set, 'wrap' text at right edge of display
    /// </summary>
    static bool textWrap = true;
    /// <summary>
    /// char width multiplicator (1 = 6, 2 = 12, 3 = 18 etc)
    /// </summary>
    static int textSizeX = 1;
    /// <summary>
    /// char height multiplicator (1 = 8, 2 = 16, 3 = 24 etc)
    /// </summary>
    static int textSizeY = 1;
    /// <summary>
    /// text color
    /// </summary>
    static uint textColor = 0xffffffff;
    /// <summary>
    /// text background color
    /// </summary>
    static uint textBackgroundColor = 0xffffffff;

    /// <summary>
    /// Font-Pixels
    /// </summary>
    static readonly byte[] Font =
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x5B, 0x4F, 0x5B, 0x3E, 0x3E, 0x6B,
      0x4F, 0x6B, 0x3E, 0x1C, 0x3E, 0x7C, 0x3E, 0x1C, 0x18, 0x3C, 0x7E, 0x3C,
      0x18, 0x1C, 0x57, 0x7D, 0x57, 0x1C, 0x1C, 0x5E, 0x7F, 0x5E, 0x1C, 0x00,
      0x18, 0x3C, 0x18, 0x00, 0xFF, 0xE7, 0xC3, 0xE7, 0xFF, 0x00, 0x18, 0x24,
      0x18, 0x00, 0xFF, 0xE7, 0xDB, 0xE7, 0xFF, 0x30, 0x48, 0x3A, 0x06, 0x0E,
      0x26, 0x29, 0x79, 0x29, 0x26, 0x40, 0x7F, 0x05, 0x05, 0x07, 0x40, 0x7F,
      0x05, 0x25, 0x3F, 0x5A, 0x3C, 0xE7, 0x3C, 0x5A, 0x7F, 0x3E, 0x1C, 0x1C,
      0x08, 0x08, 0x1C, 0x1C, 0x3E, 0x7F, 0x14, 0x22, 0x7F, 0x22, 0x14, 0x5F,
      0x5F, 0x00, 0x5F, 0x5F, 0x06, 0x09, 0x7F, 0x01, 0x7F, 0x00, 0x66, 0x89,
      0x95, 0x6A, 0x60, 0x60, 0x60, 0x60, 0x60, 0x94, 0xA2, 0xFF, 0xA2, 0x94,
      0x08, 0x04, 0x7E, 0x04, 0x08, 0x10, 0x20, 0x7E, 0x20, 0x10, 0x08, 0x08,
      0x2A, 0x1C, 0x08, 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x1E, 0x10, 0x10, 0x10,
      0x10, 0x0C, 0x1E, 0x0C, 0x1E, 0x0C, 0x30, 0x38, 0x3E, 0x38, 0x30, 0x06,
      0x0E, 0x3E, 0x0E, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F,
      0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x14, 0x7F, 0x14, 0x7F, 0x14,
      0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x23, 0x13, 0x08, 0x64, 0x62, 0x36, 0x49,
      0x56, 0x20, 0x50, 0x00, 0x08, 0x07, 0x03, 0x00, 0x00, 0x1C, 0x22, 0x41,
      0x00, 0x00, 0x41, 0x22, 0x1C, 0x00, 0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 0x08,
      0x08, 0x3E, 0x08, 0x08, 0x00, 0x80, 0x70, 0x30, 0x00, 0x08, 0x08, 0x08,
      0x08, 0x08, 0x00, 0x00, 0x60, 0x60, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02,
      0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x72, 0x49,
      0x49, 0x49, 0x46, 0x21, 0x41, 0x49, 0x4D, 0x33, 0x18, 0x14, 0x12, 0x7F,
      0x10, 0x27, 0x45, 0x45, 0x45, 0x39, 0x3C, 0x4A, 0x49, 0x49, 0x31, 0x41,
      0x21, 0x11, 0x09, 0x07, 0x36, 0x49, 0x49, 0x49, 0x36, 0x46, 0x49, 0x49,
      0x29, 0x1E, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x40, 0x34, 0x00, 0x00,
      0x00, 0x08, 0x14, 0x22, 0x41, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x41,
      0x22, 0x14, 0x08, 0x02, 0x01, 0x59, 0x09, 0x06, 0x3E, 0x41, 0x5D, 0x59,
      0x4E, 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x3E,
      0x41, 0x41, 0x41, 0x22, 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x7F, 0x49, 0x49,
      0x49, 0x41, 0x7F, 0x09, 0x09, 0x09, 0x01, 0x3E, 0x41, 0x41, 0x51, 0x73,
      0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x41, 0x7F, 0x41, 0x00, 0x20, 0x40,
      0x41, 0x3F, 0x01, 0x7F, 0x08, 0x14, 0x22, 0x41, 0x7F, 0x40, 0x40, 0x40,
      0x40, 0x7F, 0x02, 0x1C, 0x02, 0x7F, 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x3E,
      0x41, 0x41, 0x41, 0x3E, 0x7F, 0x09, 0x09, 0x09, 0x06, 0x3E, 0x41, 0x51,
      0x21, 0x5E, 0x7F, 0x09, 0x19, 0x29, 0x46, 0x26, 0x49, 0x49, 0x49, 0x32,
      0x03, 0x01, 0x7F, 0x01, 0x03, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x1F, 0x20,
      0x40, 0x20, 0x1F, 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x63, 0x14, 0x08, 0x14,
      0x63, 0x03, 0x04, 0x78, 0x04, 0x03, 0x61, 0x59, 0x49, 0x4D, 0x43, 0x00,
      0x7F, 0x41, 0x41, 0x41, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x41, 0x41,
      0x41, 0x7F, 0x04, 0x02, 0x01, 0x02, 0x04, 0x40, 0x40, 0x40, 0x40, 0x40,
      0x00, 0x03, 0x07, 0x08, 0x00, 0x20, 0x54, 0x54, 0x78, 0x40, 0x7F, 0x28,
      0x44, 0x44, 0x38, 0x38, 0x44, 0x44, 0x44, 0x28, 0x38, 0x44, 0x44, 0x28,
      0x7F, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x08, 0x7E, 0x09, 0x02, 0x18,
      0xA4, 0xA4, 0x9C, 0x78, 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x44, 0x7D,
      0x40, 0x00, 0x20, 0x40, 0x40, 0x3D, 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,
      0x00, 0x41, 0x7F, 0x40, 0x00, 0x7C, 0x04, 0x78, 0x04, 0x78, 0x7C, 0x08,
      0x04, 0x04, 0x78, 0x38, 0x44, 0x44, 0x44, 0x38, 0xFC, 0x18, 0x24, 0x24,
      0x18, 0x18, 0x24, 0x24, 0x18, 0xFC, 0x7C, 0x08, 0x04, 0x04, 0x08, 0x48,
      0x54, 0x54, 0x54, 0x24, 0x04, 0x04, 0x3F, 0x44, 0x24, 0x3C, 0x40, 0x40,
      0x20, 0x7C, 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x3C, 0x40, 0x30, 0x40, 0x3C,
      0x44, 0x28, 0x10, 0x28, 0x44, 0x4C, 0x90, 0x90, 0x90, 0x7C, 0x44, 0x64,
      0x54, 0x4C, 0x44, 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x77, 0x00,
      0x00, 0x00, 0x41, 0x36, 0x08, 0x00, 0x02, 0x01, 0x02, 0x04, 0x02, 0x3C,
      0x26, 0x23, 0x26, 0x3C, 0x1E, 0xA1, 0xA1, 0x61, 0x12, 0x3A, 0x40, 0x40,
      0x20, 0x7A, 0x38, 0x54, 0x54, 0x55, 0x59, 0x21, 0x55, 0x55, 0x79, 0x41,
      0x22, 0x54, 0x54, 0x78, 0x42, // a-umlaut
      0x21, 0x55, 0x54, 0x78, 0x40, 0x20, 0x54, 0x55, 0x79, 0x40, 0x0C, 0x1E,
      0x52, 0x72, 0x12, 0x39, 0x55, 0x55, 0x55, 0x59, 0x39, 0x54, 0x54, 0x54,
      0x59, 0x39, 0x55, 0x54, 0x54, 0x58, 0x00, 0x00, 0x45, 0x7C, 0x41, 0x00,
      0x02, 0x45, 0x7D, 0x42, 0x00, 0x01, 0x45, 0x7C, 0x40, 0x7D, 0x12, 0x11,
      0x12, 0x7D, // A-umlaut
      0xF0, 0x28, 0x25, 0x28, 0xF0, 0x7C, 0x54, 0x55, 0x45, 0x00, 0x20, 0x54,
      0x54, 0x7C, 0x54, 0x7C, 0x0A, 0x09, 0x7F, 0x49, 0x32, 0x49, 0x49, 0x49,
      0x32, 0x3A, 0x44, 0x44, 0x44, 0x3A, // o-umlaut
      0x32, 0x4A, 0x48, 0x48, 0x30, 0x3A, 0x41, 0x41, 0x21, 0x7A, 0x3A, 0x42,
      0x40, 0x20, 0x78, 0x00, 0x9D, 0xA0, 0xA0, 0x7D, 0x3D, 0x42, 0x42, 0x42,
      0x3D, // O-umlaut
      0x3D, 0x40, 0x40, 0x40, 0x3D, 0x3C, 0x24, 0xFF, 0x24, 0x24, 0x48, 0x7E,
      0x49, 0x43, 0x66, 0x2B, 0x2F, 0xFC, 0x2F, 0x2B, 0xFF, 0x09, 0x29, 0xF6,
      0x20, 0xC0, 0x88, 0x7E, 0x09, 0x03, 0x20, 0x54, 0x54, 0x79, 0x41, 0x00,
      0x00, 0x44, 0x7D, 0x41, 0x30, 0x48, 0x48, 0x4A, 0x32, 0x38, 0x40, 0x40,
      0x22, 0x7A, 0x00, 0x7A, 0x0A, 0x0A, 0x72, 0x7D, 0x0D, 0x19, 0x31, 0x7D,
      0x26, 0x29, 0x29, 0x2F, 0x28, 0x26, 0x29, 0x29, 0x29, 0x26, 0x30, 0x48,
      0x4D, 0x40, 0x20, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
      0x38, 0x2F, 0x10, 0xC8, 0xAC, 0xBA, 0x2F, 0x10, 0x28, 0x34, 0xFA, 0x00,
      0x00, 0x7B, 0x00, 0x00, 0x08, 0x14, 0x2A, 0x14, 0x22, 0x22, 0x14, 0x2A,
      0x14, 0x08, 0x55, 0x00, 0x55, 0x00, 0x55, // #176 (25% block) missing in old
                                                // code
      0xAA, 0x55, 0xAA, 0x55, 0xAA,             // 50% block
      0xFF, 0x55, 0xFF, 0x55, 0xFF,             // 75% block
      0x00, 0x00, 0x00, 0xFF, 0x00, 0x10, 0x10, 0x10, 0xFF, 0x00, 0x14, 0x14,
      0x14, 0xFF, 0x00, 0x10, 0x10, 0xFF, 0x00, 0xFF, 0x10, 0x10, 0xF0, 0x10,
      0xF0, 0x14, 0x14, 0x14, 0xFC, 0x00, 0x14, 0x14, 0xF7, 0x00, 0xFF, 0x00,
      0x00, 0xFF, 0x00, 0xFF, 0x14, 0x14, 0xF4, 0x04, 0xFC, 0x14, 0x14, 0x17,
      0x10, 0x1F, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x14, 0x14, 0x14, 0x1F, 0x00,
      0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x10,
      0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x10, 0x00, 0x00, 0x00, 0xFF,
      0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0x10, 0x00,
      0x00, 0x00, 0xFF, 0x14, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x1F,
      0x10, 0x17, 0x00, 0x00, 0xFC, 0x04, 0xF4, 0x14, 0x14, 0x17, 0x10, 0x17,
      0x14, 0x14, 0xF4, 0x04, 0xF4, 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x14, 0x14,
      0x14, 0x14, 0x14, 0x14, 0x14, 0xF7, 0x00, 0xF7, 0x14, 0x14, 0x14, 0x17,
      0x14, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x14, 0x14, 0x14, 0xF4, 0x14, 0x10,
      0x10, 0xF0, 0x10, 0xF0, 0x00, 0x00, 0x1F, 0x10, 0x1F, 0x00, 0x00, 0x00,
      0x1F, 0x14, 0x00, 0x00, 0x00, 0xFC, 0x14, 0x00, 0x00, 0xF0, 0x10, 0xF0,
      0x10, 0x10, 0xFF, 0x10, 0xFF, 0x14, 0x14, 0x14, 0xFF, 0x14, 0x10, 0x10,
      0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x38, 0x44, 0x44,
      0x38, 0x44, 0xFC, 0x4A, 0x4A, 0x4A, 0x34, // sharp-s or beta
      0x7E, 0x02, 0x02, 0x06, 0x06, 0x02, 0x7E, 0x02, 0x7E, 0x02, 0x63, 0x55,
      0x49, 0x41, 0x63, 0x38, 0x44, 0x44, 0x3C, 0x04, 0x40, 0x7E, 0x20, 0x1E,
      0x20, 0x06, 0x02, 0x7E, 0x02, 0x02, 0x99, 0xA5, 0xE7, 0xA5, 0x99, 0x1C,
      0x2A, 0x49, 0x2A, 0x1C, 0x4C, 0x72, 0x01, 0x72, 0x4C, 0x30, 0x4A, 0x4D,
      0x4D, 0x30, 0x30, 0x48, 0x78, 0x48, 0x30, 0xBC, 0x62, 0x5A, 0x46, 0x3D,
      0x3E, 0x49, 0x49, 0x49, 0x00, 0x7E, 0x01, 0x01, 0x01, 0x7E, 0x2A, 0x2A,
      0x2A, 0x2A, 0x2A, 0x44, 0x44, 0x5F, 0x44, 0x44, 0x40, 0x51, 0x4A, 0x44,
      0x40, 0x40, 0x44, 0x4A, 0x51, 0x40, 0x00, 0x00, 0xFF, 0x01, 0x03, 0xE0,
      0x80, 0xFF, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x6B, 0x08, 0x36, 0x12, 0x36,
      0x24, 0x36, 0x06, 0x0F, 0x09, 0x0F, 0x06, 0x00, 0x00, 0x18, 0x18, 0x00,
      0x00, 0x00, 0x10, 0x10, 0x00, 0x30, 0x40, 0xFF, 0x01, 0x01, 0x00, 0x1F,
      0x01, 0x01, 0x1E, 0x00, 0x19, 0x1D, 0x17, 0x12, 0x00, 0x3C, 0x3C, 0x3C,
      0x3C, 0x00, 0x00, 0x00, 0x00, 0x00 // #255 NBSP
    };

    /// <summary>
    /// draw the character
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="c">char to draw</param>
    static void drawChar(uint* ptr, byte c)
    {
      if ((textCursorX >= currentWidth) ||            // Clip right
          (textCursorY >= currentHeight) ||           // Clip bottom
          ((textCursorX + 6 * textSizeX - 1) < 0) ||  // Clip left
          ((textCursorY + 8 * textSizeY - 1) < 0))    // Clip top
        return;

      if (textColor == textBackgroundColor)
      {
        if (textSizeX * textSizeY == 1)
        {
          for (int i = 0; i < 5; i++) // Char bitmap = 5 columns
          {
            byte line = Font[c * 5 + i];
            for (int j = 0; j < 8; j++, line >>= 1)
            {
              if ((line & 1) != 0)
              {
                DrawPixel(ptr, textCursorX + i, textCursorY + j, textColor);
              }
            }
          }
        }
        else
        {
          for (int i = 0; i < 5; i++) // Char bitmap = 5 columns
          {
            byte line = Font[c * 5 + i];
            for (int j = 0; j < 8; j++, line >>= 1)
            {
              if ((line & 1) != 0)
              {
                FillRect(ptr, textCursorX + i * textSizeX, textCursorY + j * textSizeY, textSizeX, textSizeY, textColor);
              }
            }
          }
        }
      }
      else
      {
        if (textSizeX * textSizeY == 1)
        {
          for (int i = 0; i < 5; i++) // Char bitmap = 5 columns
          {
            byte line = Font[c * 5 + i];
            for (int j = 0; j < 8; j++, line >>= 1)
            {
              DrawPixel(ptr, textCursorX + i, textCursorY + j, (line & 1) != 0 ? textColor : textBackgroundColor);
            }
          }
          // If opaque, draw vertical line for last column
          FastVLine(ptr, textCursorX + 5, textCursorY, 8, textBackgroundColor);
        }
        else
        {
          for (int i = 0; i < 5; i++) // Char bitmap = 5 columns
          {
            byte line = Font[c * 5 + i];
            for (int j = 0; j < 8; j++, line >>= 1)
            {
              FillRect(ptr, textCursorX + i * textSizeX, textCursorY + j * textSizeY, textSizeX, textSizeY, (line & 1) != 0 ? textColor : textBackgroundColor);
            }
          }
          // If opaque, draw vertical line for last column
          FillRect(ptr, textCursorX + 5 * textSizeX, textCursorY, textSizeX, 8 * textSizeY, textBackgroundColor);
        }
      }
    }

    /// <summary>
    /// print a character
    /// </summary>
    /// <param name="ptr">pointer to the backbuffer</param>
    /// <param name="c">char to print</param>
    static void writeChar(uint* ptr, byte c)
    {
      if (c == '\n') // Newline?
      {
        textCursorX = 0;               // Reset x to zero,
        textCursorY += textSizeY * 8;  // advance y one line
      }
      else if (c != '\r') // Ignore carriage returns
      {
        if (textWrap && ((textCursorX + textSizeX * 6) > currentWidth)) // Off right?
        {
          textCursorX = 0;              // Reset x to zero,
          textCursorY += textSizeY * 8; // advance y one line
        }
        drawChar(ptr, c);
        textCursorX += textSizeX * 6; // Advance x one char
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
        case CmdAdafruitType.CmdNop: return 1;

        case CmdAdafruitType.CmdFillScreen:
        {
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs));
          FillScreen(p, color);
          return 1 + sizeof(ushort);
        }

        case CmdAdafruitType.CmdDrawPixel:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 2));
          DrawPixel(p, x, y, color);
          return 1 + sizeof(ushort) * 3;
        }

        case CmdAdafruitType.CmdFastHLine:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int w = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 3));
          FastHLine(p, x, y, w, color);
          return 1 + sizeof(ushort) * 4;
        }

        case CmdAdafruitType.CmdFastVLine:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int h = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 3));
          FastVLine(p, x, y, h, color);
          return 1 + sizeof(ushort) * 4;
        }

        case CmdAdafruitType.CmdDrawLine:
        {
          int x1 = BitConverter.ToInt16(buffer, bufferOfs);
          int y1 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int x2 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          int y2 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 3);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 4));
          DrawLine(p, x1, y1, x2, y2, color);
          return 1 + sizeof(ushort) * 5;
        }

        case CmdAdafruitType.CmdDrawRect:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int w = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          int h = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 3);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 4));
          FastHLine(p, x, y, w, color);
          FastHLine(p, x, y + h - 1, w, color);
          FastVLine(p, x, y, h, color);
          FastVLine(p, x + w - 1, y, h, color);
          return 1 + sizeof(ushort) * 5;
        }

        case CmdAdafruitType.CmdFillRect:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int w = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          int h = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 3);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 4));
          FillRect(p, x, y, w, h, color);
          return 1 + sizeof(ushort) * 5;
        }

        case CmdAdafruitType.CmdDrawCircle:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int r = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 3));
          DrawCircle(p, x, y, r, color);
          return 1 + sizeof(ushort) * 4;
        }

        case CmdAdafruitType.CmdFillCircle:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int r = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 3));
          FillCircle(p, x, y, r, color);
          return 1 + sizeof(ushort) * 4;
        }

        case CmdAdafruitType.CmdDrawRoundRect:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int w = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          int h = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 3);
          int r = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 4);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 5));
          int max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
          if (r > max_radius) r = max_radius;
          // smarter version
          FastHLine(p, x + r, y, w - 2 * r, color);         // Top
          FastHLine(p, x + r, y + h - 1, w - 2 * r, color); // Bottom
          FastVLine(p, x, y + r, h - 2 * r, color);         // Left
          FastVLine(p, x + w - 1, y + r, h - 2 * r, color); // Right
          // draw four corners
          DrawCircleHelper(p, x + r, y + r, r, 1, color);
          DrawCircleHelper(p, x + w - r - 1, y + r, r, 2, color);
          DrawCircleHelper(p, x + w - r - 1, y + h - r - 1, r, 4, color);
          DrawCircleHelper(p, x + r, y + h - r - 1, r, 8, color);
          return 1 + sizeof(ushort) * 6;
        }

        case CmdAdafruitType.CmdFillRoundRect:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int w = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          int h = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 3);
          int r = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 4);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 5));
          int max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
          if (r > max_radius) r = max_radius;
          // smarter version
          for (int i = 0; i < h; i++) FastHLine(p, x + r, y + i, w - 2 * r, color);
          // draw four corners
          FillCircleHelper(p, x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
          FillCircleHelper(p, x + r, y + r, r, 2, h - 2 * r - 1, color);
          return 1 + sizeof(ushort) * 6;
        }

        case CmdAdafruitType.CmdDrawTriangle:
        {
          int x1 = BitConverter.ToInt16(buffer, bufferOfs);
          int y1 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int x2 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          int y2 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 3);
          int x3 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 4);
          int y3 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 5);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 6));
          DrawLine(p, x1, y1, x2, y2, color);
          DrawLine(p, x2, y2, x3, y3, color);
          DrawLine(p, x3, y3, x1, y1, color);
          return 1 + sizeof(ushort) * 7;
        }

        case CmdAdafruitType.CmdFillTriangle:
        {
          int x1 = BitConverter.ToInt16(buffer, bufferOfs);
          int y1 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          int x2 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 2);
          int y2 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 3);
          int x3 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 4);
          int y3 = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short) * 5);
          uint color = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short) * 6));
          FillTriangle(p, x1, y1, x2, y2, x3, y3, color);
          return 1 + sizeof(ushort) * 7;
        }

        case CmdAdafruitType.CmdSetCursor:
        {
          int x = BitConverter.ToInt16(buffer, bufferOfs);
          int y = BitConverter.ToInt16(buffer, bufferOfs + sizeof(short));
          textCursorX = x;
          textCursorY = y;
          return 1 + sizeof(ushort) * 2;
        }

        case CmdAdafruitType.CmdSetTextColor:
        {
          uint color1 = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs));
          uint color2 = Color565ToArgb(BitConverter.ToUInt16(buffer, bufferOfs + sizeof(short)));
          textColor = color1;
          textBackgroundColor = color2;
          return 1 + sizeof(ushort) * 2;
        }

        case CmdAdafruitType.CmdSetTextSize:
        {
          int x = buffer[bufferOfs];
          int y = buffer[bufferOfs + 1];
          textSizeX = Math.Max(x, 1);
          textSizeY = Math.Max(y, 1);
          return 1 + sizeof(byte) * 2;
        }

        case CmdAdafruitType.CmdSetTextWrap:
        {
          byte w = buffer[bufferOfs];
          textWrap = w != 0;
          return 1 + sizeof(byte);
        }

        case CmdAdafruitType.CmdWriteChar:
        {
          byte c = buffer[bufferOfs];
          writeChar(p, c);
          return 1 + sizeof(byte);
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
