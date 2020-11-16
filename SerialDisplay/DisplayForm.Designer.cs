namespace SerialDisplay
{
  sealed partial class DisplayForm
  {
    /// <summary>
    /// Erforderliche Designervariable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Verwendete Ressourcen bereinigen.
    /// </summary>
    /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Vom Windows Form-Designer generierter Code

    /// <summary>
    /// Erforderliche Methode für die Designerunterstützung.
    /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container();
      this.displayTimer = new System.Windows.Forms.Timer(this.components);
      this.SuspendLayout();
      // 
      // displayTimer
      // 
      this.displayTimer.Enabled = true;
      this.displayTimer.Interval = 1;
      this.displayTimer.Tick += new System.EventHandler(this.displayTimer_Tick);
      // 
      // DisplayForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(983, 623);
      this.Name = "DisplayForm";
      this.Text = "Form1";
      this.Load += new System.EventHandler(this.DisplayForm_Load);
      this.Resize += new System.EventHandler(this.DisplayForm_Resize);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.Timer displayTimer;
  }
}

