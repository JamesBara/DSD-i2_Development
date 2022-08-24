using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Runtime.InteropServices;

namespace DSDi2_FPGA_tool
{
    public partial class DSDi2_FPGA_toolForm : Form
    {
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int BinaryFileOpen([MarshalAs(UnmanagedType.LPStr)] string file_path);
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int BinaryFileClose();
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int BinaryGetSize();
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int BinaryFileRead([MarshalAs(UnmanagedType.LPArray)] byte[] data, int size, int offset);
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int SetBinaryPointer(int offset);
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int ResetBinaryPointer();
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int try_to_open_com_port([MarshalAs(UnmanagedType.LPStr)] string port);
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int try_to_close_com_port([MarshalAs(UnmanagedType.LPStr)] string port);
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int ComPortAvailable();
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int ComPortWrite([MarshalAs(UnmanagedType.LPArray)] byte[] data, int size);
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int ComPortRead([MarshalAs(UnmanagedType.LPArray)] byte[] data, int size);
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int reset_bootloader();
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int reset_maple_method();
        [DllImport("10CL32.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int FlushBuf();

        private static readonly byte[] FPGA_PS_CONF = { 0xF1, 0xA5, 0x41, 0xC1 };
        private static readonly byte ACK = 0x79;
        private static readonly int fpgaBitstreamUncompressedSize = 368011; // in bytes.
        private FPGAUploadForm fpgaUploadForm;
        private string comPort = string.Empty;
        private bool isComPortOpen = false;
        private string fpgaFilePath = string.Empty;
        private int fpgaFileSize = 0;
        private bool isFileUploaded = false;
        private bool isConsoleEnabled = false;
        public DSDi2_FPGA_toolForm()
        {
            InitializeComponent();
        }

        private void DSDi2_FPGA_toolForm_Load(object sender, EventArgs e)
        {

        }

        private void DSDi2_FPGA_toolForm_Closing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("Exiting the application will restart the DSD-i2 board.\n" +
                    "Are you sure you want to exit?", "Exit", MessageBoxButtons.YesNo) == DialogResult.No)
            {
                e.Cancel = true;
            }
            else
            {
                reset_bootloader();
            }
        }

        private void RefreshButton_Click(object sender, EventArgs e)
        {
            PortComboBox.SelectedIndex = -1;
            PortComboBox.Items.Clear();
            PortComboBox.ResetText();
            foreach (string s in SerialPort.GetPortNames())
            {
                if (!PortComboBox.Items.Contains(s))
                {
                    PortComboBox.Items.Add(s);
                }
            }
            /*Automatically select the first com port if there are any*/
            if (PortComboBox.Items.Count > 0)
            {
                PortComboBox.SelectedIndex = 0;
            }
        }

        private async void ConnectButton_Click(object sender, EventArgs e)
        {

            /*Check if the user has selected an option from the combobox*/
            if (PortComboBox.SelectedIndex != -1)
            {
                comPort = PortComboBox.SelectedItem.ToString();
                if (comPort.Length > 4)
                {
                    comPort = "\\\\.\\" + comPort;
                }
            }

            if (!comPort.Equals(string.Empty))
            {
                Print($"Trying to open {comPort}...\n");
                if (await Task.Run(() => try_to_open_com_port(comPort) != 0))
                {
                    PrintError($"Failed to open {comPort}.\n");
                    comPort = string.Empty;
                    isComPortOpen = false;
                }
                else
                {
                    PrintSuccess($"{comPort} opened successfully.\n");
                    isComPortOpen = true;
                }
            }            
        }

        private void FpgaFileUploadButton_Click(object sender, EventArgs e)
        {
            if (!isComPortOpen)
            {
                PrintError($"Error. Please select a COM Port.\n");
            }
            else
            {
                FPGAOpenFileDialog.ShowDialog();
            }

        }

        private async void FpgaOpenFileDialog_FileOk(object sender, CancelEventArgs e)
        {            
            /*Get the path of specified file*/
            fpgaFilePath = FPGAOpenFileDialog.FileName;
            string fpgaFileNameOnly = FPGAOpenFileDialog.SafeFileName;
            FPGAOpenFileDialog.FileName = string.Empty;
            /*Open the handle*/
            if (await Task.Run(() => BinaryFileOpen(fpgaFilePath) != 0))
            {
                PrintError($"Error. Opening {fpgaFileNameOnly} failed.\n");
            }
            else
            {
                await Task.Run(() => ResetBinaryPointer());
                fpgaFileSize = await Task.Run(() => BinaryGetSize());
            }
            if (fpgaFileSize != fpgaBitstreamUncompressedSize)
            {
                await Task.Run(() => BinaryFileClose());
                PrintError($"Error. Wrong file size.\n");
            }
            else
            {
                fpgaUploadForm = new FPGAUploadForm();
                fpgaUploadForm.Show();
                if (!(await Task.Run(() => TryToUpload(fpgaFileNameOnly))))
                {
                    fpgaUploadForm.Close();
                    await Task.Run(() => TryToReset());
                }
                fpgaUploadForm.Close();                
            }
        }
    
        private async void SendConsole_Click(object sender, EventArgs e)
        {
            if (!ConsoleInputTextBox.Text.Equals(string.Empty))
            {
                string txt = ConsoleInputTextBox.Text.TrimEnd('\r', '\n', ' ');
                if (SendCRcharCheckBox.Checked)
                {
                    txt += "\r";
                }
                if (SendNLcharcheckBox.Checked)
                {
                    txt += "\n";
                }
                if (await Task.Run(() => ComPortWrite(Encoding.UTF8.GetBytes(txt), txt.Length)) == -1)
                {
                    PrintError($"Failed to send string.\n");
                }
                else
                {
                    ConsoleRichTextBox.AppendText($"> {ConsoleInputTextBox.Text}\n");
                    ConsoleInputTextBox.Text = string.Empty;
                    ConsoleInputTextBox.Focus();
                }
               
                int start_tick = Environment.TickCount;
                int elapsed_millisecs = 0;
                while (elapsed_millisecs < 100)
                {
                    int bytesAvailable = await Task.Run(() => ComPortAvailable());
                    if (bytesAvailable > 0)
                    {
                        byte[] dataReceived = new byte[bytesAvailable];                        
                        if (await Task.Run(() => ComPortRead(dataReceived, bytesAvailable)) == -1)
                        {
                            PrintError($"Failed to receive data.\n");
                        }
                        else
                        {
                            ConsoleRichTextBox.AppendText($"> {Encoding.UTF8.GetString(dataReceived)}\n");
                            ConsoleRichTextBox.ScrollToCaret();
                        }
                    }
                    elapsed_millisecs = Environment.TickCount - start_tick;
                }
            }
        }

        private void ClearConsole_Click(object sender, EventArgs e)
        {
            ConsoleRichTextBox.Clear();
            ConsoleRichTextBox.Focus();
        }

        private void ConsoleInputTextBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Return)
            {
                SendConsole_Click(this, new EventArgs());
            }
        }

        private void UploadFWtoNIOSButton_Click(object sender, EventArgs e)
        {

                PrintError($"Button not implemented yet.\n");
        }

        private void SRAMSIzeTextBox_TextChanged(object sender, EventArgs e)
        {
        }

        private void SRAMBaseAddrTextBox_TextChanged(object sender, EventArgs e)
        {
        }
        private  void EnableConsoleButton_Click(object sender, EventArgs e)
        {
            if (!isFileUploaded)
            {
                ConsoleRichTextBox.SelectionColor = Color.FromArgb(193, 65, 65);
                ConsoleRichTextBox.AppendText($"Error. No file uploaded.\n");
                ConsoleRichTextBox.ScrollToCaret();
            }
            else
            {
                if (!isConsoleEnabled)
                {
                    isConsoleEnabled = true;
                    EnableConsoleButton.Text = "Disable Console";
                    SendConsole.Enabled = true;
                    ConsoleInputTextBox.Enabled = true;
                    FPGAFileUploadButton.Enabled = false;
                    UploadFWtoNIOSButton.Enabled = false;
                }
                else
                {
                    isConsoleEnabled = false;
                    EnableConsoleButton.Text = "Enable Console";
                    SendConsole.Enabled = false;
                    ConsoleInputTextBox.Enabled = false;
                    FPGAFileUploadButton.Enabled = true;
                    UploadFWtoNIOSButton.Enabled = true;
                }
            }
        }

        private void Print(string text)
        {
            if (ConsoleRichTextBox.InvokeRequired)
            {
                Action safePrintSuccess = delegate { Print($"{text}"); };
                ConsoleRichTextBox.Invoke(safePrintSuccess);
            }
            else
            {
                ConsoleRichTextBox.AppendText(text);
                ConsoleRichTextBox.ScrollToCaret();
            }
        }
        private void PrintSuccess(string text)
        {
            if (ConsoleRichTextBox.InvokeRequired)
            {
                Action safePrintSuccess = delegate { PrintSuccess($"{text}"); };
                ConsoleRichTextBox.Invoke(safePrintSuccess);
            }
            else
            {
                ConsoleRichTextBox.SelectionColor = Color.FromArgb(64, 186, 64);
                ConsoleRichTextBox.AppendText(text);
                ConsoleRichTextBox.ScrollToCaret();
            }
        }

        private void PrintError(string text)
        {
            if (ConsoleRichTextBox.InvokeRequired)
            {
                Action safePrintError = delegate { PrintError($"{text}"); };
                ConsoleRichTextBox.Invoke(safePrintError);
            }
            else
            {
                ConsoleRichTextBox.SelectionColor = Color.FromArgb(193, 65, 65);
                ConsoleRichTextBox.AppendText(text);
                ConsoleRichTextBox.ScrollToCaret();
            }
        }
        private void UploadError(string text)
        {
            PrintError(text);
            BinaryFileClose();
            isFileUploaded = false;
            this.Invoke((MethodInvoker)delegate { FPGAFileSelectedLabel.Text = $"File: Nothing Uploaded Yet."; });
            fpgaUploadForm.UpdateProgressBar(0);         
        }

       private bool CheckReply(int reply_timeout)
       {
            int start_tick = Environment.TickCount;
            int elapsed_millisecs = 0;
            while (elapsed_millisecs < reply_timeout)
            {
                int bytesAvailable = ComPortAvailable();
                if (bytesAvailable > 0)
                {
                    byte[] dataReceived = new byte[1];
                    if (ComPortRead(dataReceived, dataReceived.Length) == -1)
                    {
                        return false;
                    }
                    else
                    {
                        foreach (byte data in dataReceived)
                        {
                            if (data == ACK)
                            {
                                return true;
                            }
                        }
                    }
                }
                elapsed_millisecs = Environment.TickCount - start_tick;
            }
            return false;
        }
        private bool TestForBootloader()
        {
            if (ComPortWrite(FPGA_PS_CONF, FPGA_PS_CONF.Length) == -1)
            {                
                return false;
            }
            if (!CheckReply(50))
            {
                return false;
            }
            return true;
        }

        private bool TryToUpload(string fpgaFileNameOnly)
        {
            Print($"Trying to initialize DSD-i2 bootloader...\n");
            /*Test if the bootloader is running*/
            if (!TestForBootloader())
            {
                if (reset_maple_method() == -1)
                {
                    UploadError($"Failed to initialize DSD-i2 bootloader.\n");                    
                    return false;
                }
                else
                {
                    if (try_to_close_com_port(comPort) == -1)
                    {                        
                        UploadError($"Failed to initialize DSD-i2 bootloader.\n");
                        return false;
                    }
                    if (try_to_open_com_port(comPort) == -1)
                    {                        
                        UploadError($"Failed to initialize DSD-i2 bootloader.\n");
                        return false;
                    }
                    if (!TestForBootloader())
                    {
                        UploadError($"Failed to initialize DSD-i2 bootloader.\n");
                        return false;
                    }
                }
            }
            PrintSuccess($"DSD-i2 bootloader initialized.\n");
            Print($"Sending the size of the {fpgaFileNameOnly} file...\n");
            /*Bootloader should be up and running by this point.*/
            /*Send the file size.*/
            byte[] fpgaFileSizeInBytes = BitConverter.GetBytes(fpgaBitstreamUncompressedSize);
            Array.Reverse(fpgaFileSizeInBytes);

            if (ComPortWrite(fpgaFileSizeInBytes, fpgaFileSizeInBytes.Length) == -1)
            {                
                UploadError($"Failed to send the size.\n");
                return false;
            }
            if (!CheckReply(50))
            {                
                UploadError($"Failed to send the size.\n");
                return false;
            }
            PrintSuccess($"Size of {fpgaFileNameOnly} file was sent succesfully.\n");
            Print("Setting FPGA in configuration mode...\n");
            if (!CheckReply(150))
            {
                UploadError($"Failed to set FPGA in configuration mode.\n");
                return false;
            }
            Print("Uploading...\n");
            int i = 0;
            for (; i < (fpgaFileSize / 64); i++)
            {
                byte[] dataToSend = new byte[64];

                if (BinaryFileRead(dataToSend, dataToSend.Length, i * 64) == -1)
                {
                    UploadError($"Error. Upload failed.\n");
                    return false;
                }
                else
                {
                    if (ComPortWrite(dataToSend, dataToSend.Length) == -1)
                    {
                        UploadError($"Error. Upload failed.\n");
                        return false;
                    }
                    else
                    {
                        if (CheckReply(50))
                        {                            
                            UploadError($"Error. Upload failed.\n");
                            return false;
                        }
                        else
                        {
                            fpgaUploadForm.UpdateProgressBar((int)(100 * Math.Floor((double)(i + 1) / (double)(fpgaFileSize / 64))));
                        }
                    }
                }
            }

            /*Handle the leftover bytes*/
            if ((fpgaFileSize % 64) != 0)
            {
                byte[] dataToSend = new byte[fpgaFileSize % 64];
                if (BinaryFileRead(dataToSend, dataToSend.Length, i * 64) == -1)
                {
                    UploadError($"Error. Upload failed.\n");
                    return false;
                }
                else
                {
                    if (ComPortWrite(dataToSend, dataToSend.Length) == -1)
                    {
                        UploadError($"Error. Upload failed.\n");
                        return false;
                    }
                    else
                    {
                        if (!CheckReply(50))
                        {
                            UploadError($"Error. Upload failed.\n");
                            return false;
                        }
                        else
                        {
                            fpgaUploadForm.UpdateProgressBar(100);
                            PrintSuccess($"Uploaded {fpgaFileSize} bytes to the FPGA.\n");
                            Print($"Initializating FPGA...\n");
                            if (!CheckReply(20000))
                            {
                                UploadError($"FPGA Initialization timeout.\n");
                                return false;
                            }
                            else
                            {
                                /*upload done*/
                                BinaryFileClose();
                                PrintSuccess($"FPGA initialized successfully.\n");
                                PrintSuccess($"FPGA configuration completed!\n");
                                isFileUploaded = true;
                                this.Invoke((MethodInvoker)delegate { FPGAFileSelectedLabel.Text = $"File: {fpgaFilePath}"; });
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }
        private bool TryToReset()
        {
            Print($"Trying to reset DSD-i2 board...\n");
            /*if the upload fails attempt to reset the board*/
            if (reset_bootloader() == -1)
            {
                PrintError($"Failed to reset the board.\n");
                Print($"Please try to reset the DSD-i2 board manually, by using the STM_RST button.\n");
                return false;
            }
            else
            {
               if (try_to_close_com_port(comPort)==-1)
                {
                    PrintError($"Failed to reset the board.\n");
                    Print($"Please try to reset the DSD-i2 board manually, by using the STM_RST button.\n");
                    return false;
                }   
            }
            isComPortOpen = false;
            PrintSuccess($"DSD-i2 board reset was successful.\n");
            Print($"Please try to reconnect.\n");
            return true;
        }

    }
}