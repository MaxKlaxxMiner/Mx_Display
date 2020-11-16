
namespace SerialDisplay
{
  public enum DisplayCmdType : byte
  {
    /// <summary>
    /// [1] - get display info
    /// </summary>
    GetInfo = 0x00,
    /// <summary>
    /// [1 + 2] - select 16-Bit color (red: 5-Bit, green: 6-Bit, blue: 5-Bit)
    /// </summary>
    SelectColor565 = 0x01,
    /// <summary>
    /// [1 + 3] - select 24-Bit true-color (red: 8-Bit, green: 8-Bit, blue: 8-Bit)
    /// </summary>
    SelectColorTrue = 0x02,
    /// <summary>
    /// [1 + 2 + 2] - draw pixel with current color
    /// </summary>
    DrawPixel = 0x03,
    /// <summary>
    /// [1 + 2 + 2 + 2] - draw horizontal line with current color
    /// </summary>
    DrawHLine = 0x04,
    /// <summary>
    /// [1 + 2 + 2 + 2] - draw vertical line with current color
    /// </summary>
    DrawVLine = 0x05,
    /// <summary>
    /// [1 + 2 + 2 + 2 + 2] - draw normal line with current color
    /// </summary>
    DrawLine = 0x06,
  }
}
