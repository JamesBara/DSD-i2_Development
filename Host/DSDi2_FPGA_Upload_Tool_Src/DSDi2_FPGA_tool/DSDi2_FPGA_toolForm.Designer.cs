
namespace DSDi2_FPGA_tool
{
    partial class DSDi2_FPGA_toolForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DSDi2_FPGA_toolForm));
            this.PortPanel = new System.Windows.Forms.Panel();
            this.ConnectButton = new System.Windows.Forms.Button();
            this.PortLabel = new System.Windows.Forms.Label();
            this.RefreshButton = new System.Windows.Forms.Button();
            this.PortComboBox = new System.Windows.Forms.ComboBox();
            this.FPGAOpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.ConsoleRichTextBox = new System.Windows.Forms.RichTextBox();
            this.ConsoleOutputLabel = new System.Windows.Forms.Label();
            this.ClearConsole = new System.Windows.Forms.Button();
            this.SendConsole = new System.Windows.Forms.Button();
            this.consoleInputLabel = new System.Windows.Forms.Label();
            this.consolePanel = new System.Windows.Forms.Panel();
            this.ConsoleInputTextBox = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.SRAMSizeTextBox = new System.Windows.Forms.TextBox();
            this.SRAMBaseAddrTextBox = new System.Windows.Forms.TextBox();
            this.SRAMSizeBaseAddrLabel = new System.Windows.Forms.Label();
            this.SRAMSizeLabel = new System.Windows.Forms.Label();
            this.consoleOptionsPanel = new System.Windows.Forms.Panel();
            this.SendNLcharcheckBox = new System.Windows.Forms.CheckBox();
            this.SendCRcharCheckBox = new System.Windows.Forms.CheckBox();
            this.NiosSettingsLabel = new System.Windows.Forms.Label();
            this.ConsoleOptionsLabel = new System.Windows.Forms.Label();
            this.FPGAFileSelectedLabel = new System.Windows.Forms.Label();
            this.FPGAFileUploadButton = new System.Windows.Forms.Button();
            this.EnableConsoleButton = new System.Windows.Forms.Button();
            this.UploadFWtoNIOSButton = new System.Windows.Forms.Button();
            this.FPGAPanel = new System.Windows.Forms.Panel();
            this.PortPanel.SuspendLayout();
            this.consolePanel.SuspendLayout();
            this.panel1.SuspendLayout();
            this.consoleOptionsPanel.SuspendLayout();
            this.FPGAPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // PortPanel
            // 
            this.PortPanel.AutoSize = true;
            this.PortPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.PortPanel.Controls.Add(this.ConnectButton);
            this.PortPanel.Controls.Add(this.PortLabel);
            this.PortPanel.Controls.Add(this.RefreshButton);
            this.PortPanel.Controls.Add(this.PortComboBox);
            this.PortPanel.Location = new System.Drawing.Point(0, 0);
            this.PortPanel.Name = "PortPanel";
            this.PortPanel.Size = new System.Drawing.Size(742, 42);
            this.PortPanel.TabIndex = 0;
            // 
            // ConnectButton
            // 
            this.ConnectButton.BackColor = System.Drawing.SystemColors.ControlLight;
            this.ConnectButton.Font = new System.Drawing.Font("Segoe UI", 12F);
            this.ConnectButton.ForeColor = System.Drawing.Color.Black;
            this.ConnectButton.Location = new System.Drawing.Point(577, 6);
            this.ConnectButton.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(162, 32);
            this.ConnectButton.TabIndex = 3;
            this.ConnectButton.Text = "Connect";
            this.ConnectButton.UseVisualStyleBackColor = false;
            this.ConnectButton.Click += new System.EventHandler(this.ConnectButton_Click);
            // 
            // PortLabel
            // 
            this.PortLabel.BackColor = System.Drawing.Color.Transparent;
            this.PortLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.PortLabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.PortLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.PortLabel.Location = new System.Drawing.Point(0, 5);
            this.PortLabel.Name = "PortLabel";
            this.PortLabel.Size = new System.Drawing.Size(113, 32);
            this.PortLabel.TabIndex = 0;
            this.PortLabel.Text = "COM Port:";
            this.PortLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // RefreshButton
            // 
            this.RefreshButton.BackColor = System.Drawing.SystemColors.ControlLight;
            this.RefreshButton.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(161)));
            this.RefreshButton.Image = ((System.Drawing.Image)(resources.GetObject("RefreshButton.Image")));
            this.RefreshButton.Location = new System.Drawing.Point(524, 5);
            this.RefreshButton.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.RefreshButton.Name = "RefreshButton";
            this.RefreshButton.Size = new System.Drawing.Size(47, 32);
            this.RefreshButton.TabIndex = 2;
            this.RefreshButton.UseVisualStyleBackColor = false;
            this.RefreshButton.Click += new System.EventHandler(this.RefreshButton_Click);
            // 
            // PortComboBox
            // 
            this.PortComboBox.BackColor = System.Drawing.SystemColors.ControlLight;
            this.PortComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.PortComboBox.Font = new System.Drawing.Font("Segoe UI", 13F);
            this.PortComboBox.ForeColor = System.Drawing.Color.Black;
            this.PortComboBox.FormattingEnabled = true;
            this.PortComboBox.Location = new System.Drawing.Point(119, 5);
            this.PortComboBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.PortComboBox.Name = "PortComboBox";
            this.PortComboBox.Size = new System.Drawing.Size(399, 31);
            this.PortComboBox.TabIndex = 4;
            // 
            // FPGAOpenFileDialog
            // 
            this.FPGAOpenFileDialog.Filter = "Intel Files (*.rbf)|*.rbf";
            this.FPGAOpenFileDialog.InitialDirectory = "c:\\";
            this.FPGAOpenFileDialog.RestoreDirectory = true;
            this.FPGAOpenFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.FpgaOpenFileDialog_FileOk);
            // 
            // ConsoleRichTextBox
            // 
            this.ConsoleRichTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
            this.ConsoleRichTextBox.Cursor = System.Windows.Forms.Cursors.Default;
            this.ConsoleRichTextBox.DetectUrls = false;
            this.ConsoleRichTextBox.Font = new System.Drawing.Font("Segoe UI", 12F);
            this.ConsoleRichTextBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.ConsoleRichTextBox.HideSelection = false;
            this.ConsoleRichTextBox.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.ConsoleRichTextBox.Location = new System.Drawing.Point(0, 256);
            this.ConsoleRichTextBox.Name = "ConsoleRichTextBox";
            this.ConsoleRichTextBox.ReadOnly = true;
            this.ConsoleRichTextBox.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.ConsoleRichTextBox.Size = new System.Drawing.Size(742, 176);
            this.ConsoleRichTextBox.TabIndex = 10;
            this.ConsoleRichTextBox.TabStop = false;
            this.ConsoleRichTextBox.Text = "";
            // 
            // ConsoleOutputLabel
            // 
            this.ConsoleOutputLabel.BackColor = System.Drawing.Color.Transparent;
            this.ConsoleOutputLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ConsoleOutputLabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.ConsoleOutputLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.ConsoleOutputLabel.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.ConsoleOutputLabel.Location = new System.Drawing.Point(0, 230);
            this.ConsoleOutputLabel.Name = "ConsoleOutputLabel";
            this.ConsoleOutputLabel.Size = new System.Drawing.Size(742, 23);
            this.ConsoleOutputLabel.TabIndex = 9;
            this.ConsoleOutputLabel.Text = "Console Output:";
            this.ConsoleOutputLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // ClearConsole
            // 
            this.ClearConsole.BackColor = System.Drawing.SystemColors.ControlLight;
            this.ClearConsole.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(161)));
            this.ClearConsole.Location = new System.Drawing.Point(577, 3);
            this.ClearConsole.Name = "ClearConsole";
            this.ClearConsole.Size = new System.Drawing.Size(162, 32);
            this.ClearConsole.TabIndex = 12;
            this.ClearConsole.Text = "Clear";
            this.ClearConsole.UseVisualStyleBackColor = false;
            this.ClearConsole.Click += new System.EventHandler(this.ClearConsole_Click);
            // 
            // SendConsole
            // 
            this.SendConsole.BackColor = System.Drawing.SystemColors.ControlLight;
            this.SendConsole.Enabled = false;
            this.SendConsole.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(161)));
            this.SendConsole.Location = new System.Drawing.Point(409, 3);
            this.SendConsole.Name = "SendConsole";
            this.SendConsole.Size = new System.Drawing.Size(162, 32);
            this.SendConsole.TabIndex = 13;
            this.SendConsole.Text = "Send";
            this.SendConsole.UseVisualStyleBackColor = false;
            this.SendConsole.Click += new System.EventHandler(this.SendConsole_Click);
            // 
            // consoleInputLabel
            // 
            this.consoleInputLabel.BackColor = System.Drawing.Color.Transparent;
            this.consoleInputLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.consoleInputLabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.consoleInputLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.consoleInputLabel.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
            this.consoleInputLabel.Location = new System.Drawing.Point(0, 438);
            this.consoleInputLabel.Name = "consoleInputLabel";
            this.consoleInputLabel.Size = new System.Drawing.Size(742, 23);
            this.consoleInputLabel.TabIndex = 15;
            this.consoleInputLabel.Text = "Console Input:";
            this.consoleInputLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // consolePanel
            // 
            this.consolePanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.consolePanel.Controls.Add(this.SendConsole);
            this.consolePanel.Controls.Add(this.ConsoleInputTextBox);
            this.consolePanel.Controls.Add(this.ClearConsole);
            this.consolePanel.Location = new System.Drawing.Point(0, 464);
            this.consolePanel.Name = "consolePanel";
            this.consolePanel.Size = new System.Drawing.Size(742, 38);
            this.consolePanel.TabIndex = 12;
            // 
            // ConsoleInputTextBox
            // 
            this.ConsoleInputTextBox.BackColor = System.Drawing.SystemColors.ControlLight;
            this.ConsoleInputTextBox.Enabled = false;
            this.ConsoleInputTextBox.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.ConsoleInputTextBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.ConsoleInputTextBox.Location = new System.Drawing.Point(0, 3);
            this.ConsoleInputTextBox.Name = "ConsoleInputTextBox";
            this.ConsoleInputTextBox.Size = new System.Drawing.Size(403, 32);
            this.ConsoleInputTextBox.TabIndex = 14;
            this.ConsoleInputTextBox.WordWrap = false;
            this.ConsoleInputTextBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ConsoleInputTextBox_KeyPress);
            // 
            // panel1
            // 
            this.panel1.AutoSize = true;
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.panel1.Controls.Add(this.SRAMSizeTextBox);
            this.panel1.Controls.Add(this.SRAMBaseAddrTextBox);
            this.panel1.Controls.Add(this.SRAMSizeBaseAddrLabel);
            this.panel1.Controls.Add(this.SRAMSizeLabel);
            this.panel1.Location = new System.Drawing.Point(0, 72);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(254, 82);
            this.panel1.TabIndex = 10;
            // 
            // SRAMSizeTextBox
            // 
            this.SRAMSizeTextBox.BackColor = System.Drawing.SystemColors.ControlLight;
            this.SRAMSizeTextBox.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.SRAMSizeTextBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.SRAMSizeTextBox.Location = new System.Drawing.Point(166, 5);
            this.SRAMSizeTextBox.Name = "SRAMSizeTextBox";
            this.SRAMSizeTextBox.Size = new System.Drawing.Size(83, 32);
            this.SRAMSizeTextBox.TabIndex = 15;
            this.SRAMSizeTextBox.Text = "33750";
            this.SRAMSizeTextBox.TextChanged += new System.EventHandler(this.SRAMSIzeTextBox_TextChanged);
            // 
            // SRAMBaseAddrTextBox
            // 
            this.SRAMBaseAddrTextBox.BackColor = System.Drawing.SystemColors.ControlLight;
            this.SRAMBaseAddrTextBox.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.SRAMBaseAddrTextBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.SRAMBaseAddrTextBox.Location = new System.Drawing.Point(166, 46);
            this.SRAMBaseAddrTextBox.Name = "SRAMBaseAddrTextBox";
            this.SRAMBaseAddrTextBox.Size = new System.Drawing.Size(83, 32);
            this.SRAMBaseAddrTextBox.TabIndex = 16;
            this.SRAMBaseAddrTextBox.Text = "00000";
            this.SRAMBaseAddrTextBox.TextChanged += new System.EventHandler(this.SRAMBaseAddrTextBox_TextChanged);
            // 
            // SRAMSizeBaseAddrLabel
            // 
            this.SRAMSizeBaseAddrLabel.BackColor = System.Drawing.Color.Transparent;
            this.SRAMSizeBaseAddrLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.SRAMSizeBaseAddrLabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.SRAMSizeBaseAddrLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.SRAMSizeBaseAddrLabel.Location = new System.Drawing.Point(3, 45);
            this.SRAMSizeBaseAddrLabel.Name = "SRAMSizeBaseAddrLabel";
            this.SRAMSizeBaseAddrLabel.Size = new System.Drawing.Size(157, 32);
            this.SRAMSizeBaseAddrLabel.TabIndex = 17;
            this.SRAMSizeBaseAddrLabel.Text = "SRAM Base Addr:";
            this.SRAMSizeBaseAddrLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // SRAMSizeLabel
            // 
            this.SRAMSizeLabel.BackColor = System.Drawing.Color.Transparent;
            this.SRAMSizeLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.SRAMSizeLabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.SRAMSizeLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.SRAMSizeLabel.Location = new System.Drawing.Point(3, 4);
            this.SRAMSizeLabel.Name = "SRAMSizeLabel";
            this.SRAMSizeLabel.Size = new System.Drawing.Size(107, 32);
            this.SRAMSizeLabel.TabIndex = 16;
            this.SRAMSizeLabel.Text = "SRAM Size:";
            this.SRAMSizeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // consoleOptionsPanel
            // 
            this.consoleOptionsPanel.AutoSize = true;
            this.consoleOptionsPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.consoleOptionsPanel.Controls.Add(this.SendNLcharcheckBox);
            this.consoleOptionsPanel.Controls.Add(this.SendCRcharCheckBox);
            this.consoleOptionsPanel.Location = new System.Drawing.Point(535, 76);
            this.consoleOptionsPanel.Name = "consoleOptionsPanel";
            this.consoleOptionsPanel.Size = new System.Drawing.Size(207, 78);
            this.consoleOptionsPanel.TabIndex = 11;
            // 
            // SendNLcharcheckBox
            // 
            this.SendNLcharcheckBox.AutoSize = true;
            this.SendNLcharcheckBox.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.SendNLcharcheckBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.SendNLcharcheckBox.Location = new System.Drawing.Point(10, 44);
            this.SendNLcharcheckBox.Name = "SendNLcharcheckBox";
            this.SendNLcharcheckBox.Size = new System.Drawing.Size(185, 29);
            this.SendNLcharcheckBox.TabIndex = 8;
            this.SendNLcharcheckBox.Text = "Send NL character";
            this.SendNLcharcheckBox.UseVisualStyleBackColor = true;
            // 
            // SendCRcharCheckBox
            // 
            this.SendCRcharCheckBox.AutoSize = true;
            this.SendCRcharCheckBox.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.SendCRcharCheckBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.SendCRcharCheckBox.Location = new System.Drawing.Point(10, 4);
            this.SendCRcharCheckBox.Name = "SendCRcharCheckBox";
            this.SendCRcharCheckBox.Size = new System.Drawing.Size(185, 29);
            this.SendCRcharCheckBox.TabIndex = 7;
            this.SendCRcharCheckBox.Text = "Send CR character";
            this.SendCRcharCheckBox.UseVisualStyleBackColor = true;
            // 
            // NiosSettingsLabel
            // 
            this.NiosSettingsLabel.BackColor = System.Drawing.Color.Transparent;
            this.NiosSettingsLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.NiosSettingsLabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.NiosSettingsLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.NiosSettingsLabel.Location = new System.Drawing.Point(0, 37);
            this.NiosSettingsLabel.Name = "NiosSettingsLabel";
            this.NiosSettingsLabel.Size = new System.Drawing.Size(254, 32);
            this.NiosSettingsLabel.TabIndex = 5;
            this.NiosSettingsLabel.Text = "NIOS Settings";
            this.NiosSettingsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ConsoleOptionsLabel
            // 
            this.ConsoleOptionsLabel.BackColor = System.Drawing.Color.Transparent;
            this.ConsoleOptionsLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ConsoleOptionsLabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.ConsoleOptionsLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.ConsoleOptionsLabel.Location = new System.Drawing.Point(530, 41);
            this.ConsoleOptionsLabel.Name = "ConsoleOptionsLabel";
            this.ConsoleOptionsLabel.Size = new System.Drawing.Size(212, 32);
            this.ConsoleOptionsLabel.TabIndex = 6;
            this.ConsoleOptionsLabel.Text = "Console Options";
            this.ConsoleOptionsLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // FPGAFileSelectedLabel
            // 
            this.FPGAFileSelectedLabel.AutoEllipsis = true;
            this.FPGAFileSelectedLabel.BackColor = System.Drawing.Color.Transparent;
            this.FPGAFileSelectedLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.FPGAFileSelectedLabel.Font = new System.Drawing.Font("Segoe UI", 14F);
            this.FPGAFileSelectedLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(158)))), ((int)(((byte)(161)))), ((int)(((byte)(178)))));
            this.FPGAFileSelectedLabel.Location = new System.Drawing.Point(0, 0);
            this.FPGAFileSelectedLabel.MinimumSize = new System.Drawing.Size(299, 24);
            this.FPGAFileSelectedLabel.Name = "FPGAFileSelectedLabel";
            this.FPGAFileSelectedLabel.Size = new System.Drawing.Size(745, 29);
            this.FPGAFileSelectedLabel.TabIndex = 6;
            this.FPGAFileSelectedLabel.Text = "File: Nothing Uploaded Yet.";
            this.FPGAFileSelectedLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // FPGAFileUploadButton
            // 
            this.FPGAFileUploadButton.BackColor = System.Drawing.SystemColors.ControlLight;
            this.FPGAFileUploadButton.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(161)));
            this.FPGAFileUploadButton.Location = new System.Drawing.Point(3, 32);
            this.FPGAFileUploadButton.Name = "FPGAFileUploadButton";
            this.FPGAFileUploadButton.Size = new System.Drawing.Size(162, 32);
            this.FPGAFileUploadButton.TabIndex = 7;
            this.FPGAFileUploadButton.Text = "Upload BitStream";
            this.FPGAFileUploadButton.UseVisualStyleBackColor = false;
            this.FPGAFileUploadButton.Click += new System.EventHandler(this.FpgaFileUploadButton_Click);
            // 
            // EnableConsoleButton
            // 
            this.EnableConsoleButton.BackColor = System.Drawing.SystemColors.ControlLight;
            this.EnableConsoleButton.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(161)));
            this.EnableConsoleButton.Location = new System.Drawing.Point(577, 32);
            this.EnableConsoleButton.Name = "EnableConsoleButton";
            this.EnableConsoleButton.Size = new System.Drawing.Size(162, 32);
            this.EnableConsoleButton.TabIndex = 8;
            this.EnableConsoleButton.Text = "Enable Console";
            this.EnableConsoleButton.UseVisualStyleBackColor = false;
            this.EnableConsoleButton.Click += new System.EventHandler(this.EnableConsoleButton_Click);
            // 
            // UploadFWtoNIOSButton
            // 
            this.UploadFWtoNIOSButton.BackColor = System.Drawing.SystemColors.ControlLight;
            this.UploadFWtoNIOSButton.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(161)));
            this.UploadFWtoNIOSButton.Location = new System.Drawing.Point(171, 32);
            this.UploadFWtoNIOSButton.Name = "UploadFWtoNIOSButton";
            this.UploadFWtoNIOSButton.Size = new System.Drawing.Size(162, 32);
            this.UploadFWtoNIOSButton.TabIndex = 9;
            this.UploadFWtoNIOSButton.Text = "Upload FW to NIOS";
            this.UploadFWtoNIOSButton.UseVisualStyleBackColor = false;
            this.UploadFWtoNIOSButton.Click += new System.EventHandler(this.UploadFWtoNIOSButton_Click);
            // 
            // FPGAPanel
            // 
            this.FPGAPanel.AutoSize = true;
            this.FPGAPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.FPGAPanel.Controls.Add(this.UploadFWtoNIOSButton);
            this.FPGAPanel.Controls.Add(this.EnableConsoleButton);
            this.FPGAPanel.Controls.Add(this.FPGAFileUploadButton);
            this.FPGAPanel.Controls.Add(this.FPGAFileSelectedLabel);
            this.FPGAPanel.Location = new System.Drawing.Point(0, 160);
            this.FPGAPanel.Name = "FPGAPanel";
            this.FPGAPanel.Size = new System.Drawing.Size(748, 67);
            this.FPGAPanel.TabIndex = 5;
            // 
            // DSDi2_FPGA_toolForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(66)))), ((int)(((byte)(94)))));
            this.ClientSize = new System.Drawing.Size(742, 501);
            this.Controls.Add(this.NiosSettingsLabel);
            this.Controls.Add(this.ConsoleOptionsLabel);
            this.Controls.Add(this.consoleOptionsPanel);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.consoleInputLabel);
            this.Controls.Add(this.ConsoleOutputLabel);
            this.Controls.Add(this.consolePanel);
            this.Controls.Add(this.ConsoleRichTextBox);
            this.Controls.Add(this.FPGAPanel);
            this.Controls.Add(this.PortPanel);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(161)));
            this.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(36)))), ((int)(((byte)(64)))));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(402, 400);
            this.Name = "DSDi2_FPGA_toolForm";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "DSD-i2 FPGA Upload Tool";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DSDi2_FPGA_toolForm_Closing);
            this.Load += new System.EventHandler(this.DSDi2_FPGA_toolForm_Load);
            this.PortPanel.ResumeLayout(false);
            this.consolePanel.ResumeLayout(false);
            this.consolePanel.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.consoleOptionsPanel.ResumeLayout(false);
            this.consoleOptionsPanel.PerformLayout();
            this.FPGAPanel.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel PortPanel;
        private System.Windows.Forms.Label PortLabel;
        private System.Windows.Forms.Button RefreshButton;
        private System.Windows.Forms.Button ConnectButton;
        private System.Windows.Forms.ComboBox PortComboBox;
        private System.Windows.Forms.OpenFileDialog FPGAOpenFileDialog;
        private System.Windows.Forms.RichTextBox ConsoleRichTextBox;
        private System.Windows.Forms.Label ConsoleOutputLabel;
        private System.Windows.Forms.Button ClearConsole;
        private System.Windows.Forms.Button SendConsole;
        private System.Windows.Forms.Label consoleInputLabel;
        private System.Windows.Forms.Panel consolePanel;
        private System.Windows.Forms.TextBox ConsoleInputTextBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel consoleOptionsPanel;
        private System.Windows.Forms.Label NiosSettingsLabel;
        private System.Windows.Forms.Label ConsoleOptionsLabel;
        private System.Windows.Forms.CheckBox SendNLcharcheckBox;
        private System.Windows.Forms.CheckBox SendCRcharCheckBox;
        private System.Windows.Forms.Label SRAMSizeBaseAddrLabel;
        private System.Windows.Forms.Label SRAMSizeLabel;
        private System.Windows.Forms.TextBox SRAMSizeTextBox;
        private System.Windows.Forms.Label FPGAFileSelectedLabel;
        private System.Windows.Forms.Button FPGAFileUploadButton;
        private System.Windows.Forms.Button EnableConsoleButton;
        private System.Windows.Forms.Button UploadFWtoNIOSButton;
        private System.Windows.Forms.Panel FPGAPanel;
        private System.Windows.Forms.TextBox SRAMBaseAddrTextBox;
    }
}

