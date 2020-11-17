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
        case CmdAdafruitType.CmdDrawTriangle:
        case CmdAdafruitType.CmdFillTriangle: return 1 + sizeof(ushort) * 7;
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
          for (int i = 0; i < h; i++) FastHLine(p, x, y + i, w, color);
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
