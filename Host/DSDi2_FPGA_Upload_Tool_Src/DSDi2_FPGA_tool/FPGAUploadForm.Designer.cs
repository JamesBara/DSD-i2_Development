
namespace DSDi2_FPGA_tool
{
    partial class FPGAUploadForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.uploadingToFPGALabel = new System.Windows.Forms.Label();
            this.fpgaProgressBar = new System.Windows.Forms.ProgressBar();
            this.SuspendLayout();
            // 
            // uploadingToFPGALabel
            // 
            this.uploadingToFPGALabel.BackColor = System.Drawing.Color.Transparent;
            this.uploadingToFPGALabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.uploadingToFPGALabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.uploadingToFPGALabel.Location = new System.Drawing.Point(12, 0);
            this.uploadingToFPGALabel.Name = "uploadingToFPGALabel";
            this.uploadingToFPGALabel.Size = new System.Drawing.Size(375, 25);
            this.uploadingToFPGALabel.TabIndex = 0;
            this.uploadingToFPGALabel.Text = "Uploading to FPGA...";
            this.uploadingToFPGALabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // fpgaProgressBar
            // 
            this.fpgaProgressBar.ForeColor = System.Drawing.Color.MediumSeaGreen;
            this.fpgaProgressBar.Location = new System.Drawing.Point(12, 28);
            this.fpgaProgressBar.Name = "fpgaProgressBar";
            this.fpgaProgressBar.Size = new System.Drawing.Size(375, 23);
            this.fpgaProgressBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.fpgaProgressBar.TabIndex = 1;
            // 
            // FPGAUploadForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(66)))), ((int)(((byte)(94)))));
            this.ClientSize = new System.Drawing.Size(399, 76);
            this.ControlBox = false;
            this.Controls.Add(this.fpgaProgressBar);
            this.Controls.Add(this.uploadingToFPGALabel);
            this.Font = new System.Drawing.Font("Segoe UI", 8.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FPGAUploadForm";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FPGAUploadForm";
            this.Load += new System.EventHandler(this.FPGAUploadForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label uploadingToFPGALabel;
        private System.Windows.Forms.ProgressBar fpgaProgressBar;
    }
}