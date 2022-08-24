using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DSDi2_FPGA_tool
{
    public partial class FPGAUploadForm : Form
    {
        public FPGAUploadForm()
        {
            InitializeComponent();
        }

        private void FPGAUploadForm_Load(object sender, EventArgs e)
        {

        }
        public void UpdateProgressBar(int value)
        {
            if (fpgaProgressBar.InvokeRequired)
            {
                Action safeFpgaProgressBar = delegate { UpdateProgressBar(value); };
                fpgaProgressBar.Invoke(safeFpgaProgressBar);
            }
            else
            {
                fpgaProgressBar.Value = value;
            }            
        }
    }
}
