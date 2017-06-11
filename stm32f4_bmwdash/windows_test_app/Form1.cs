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

namespace bmwSerial
{
    public partial class Form1 : Form
    {

        private SerialPort comPort = new SerialPort();
        private int rpm = 0;
        private int spd = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void findPorts()
        {
            string[] list = SerialPort.GetPortNames();
            cbPorts.Items.Clear();
            for (int i = 0; i <= list.GetUpperBound(0); i++) cbPorts.Items.Add(list[i]);
            Array.Sort(list);
            cbPorts.Text = list[0];
        }

        private void sendData(int rpm, int spd, int left, int right, int high)
        {
            int r4 = rpm / 1000;
            int r3 = (rpm / 100) % 10;
            int r2 = (rpm / 10) % 10;
            int r1 = rpm % 10;
            int s3 = spd / 100;
            int s2 = (spd / 10) % 10;
            int s1 = spd % 10;
            try
            {
                comPort.Write("$");
                comPort.Write(r4.ToString());
                comPort.Write(r3.ToString());
                comPort.Write(r2.ToString());
                comPort.Write(r1.ToString());
                comPort.Write(" ");
                comPort.Write(s3.ToString());
                comPort.Write(s2.ToString());
                comPort.Write(s1.ToString());
                comPort.Write(" ");
                comPort.Write(left.ToString());
                comPort.Write(" ");
                comPort.Write(right.ToString());
                comPort.Write(" ");
                comPort.Write(high.ToString());
                comPort.Write("|");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            findPorts();
        }

        private void btnRescan_Click(object sender, EventArgs e)
        {
            findPorts();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (btnConnect.Text.Equals("Connect"))
            {
                if (comPort.IsOpen) comPort.Close();
                comPort.PortName = Convert.ToString(cbPorts.Text);
                comPort.BaudRate = 9600;
                comPort.DataBits = 8;
                comPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), "One");
                comPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), "None");
                comPort.Parity = (Parity)Enum.Parse(typeof(Parity), "None");
                try
                {
                    comPort.Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
                btnConnect.BackColor = Color.SpringGreen;
                btnConnect.Text = "Disconnect";
            }
            else
            {
                if (comPort.IsOpen) comPort.Close();
                btnConnect.BackColor = SystemColors.Control;
                btnConnect.Text = "Connect";
            }
        }

        private void btnSignalLeft_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            if (btnSignalLeft.BackColor == SystemColors.Control)
            {
                btnSignalLeft.BackColor = Color.SandyBrown;
                sendData(rpm, spd, 1, 0, 0);
            }
            else
            {
                btnSignalLeft.BackColor = SystemColors.Control;
                sendData(rpm, spd, 2, 0, 0);
            }
            
        }

        private void btnHighBeam_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            if (btnHighBeam.BackColor == SystemColors.Control)
            {
                btnHighBeam.BackColor = Color.DeepSkyBlue;
                sendData(rpm, spd, 0, 0, 1);
            }
            else
            {
                btnHighBeam.BackColor = SystemColors.Control;
                sendData(rpm, spd, 0, 0, 2);
            }
        }

        private void btnSignalRight_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            if (btnSignalRight.BackColor == SystemColors.Control)
            {
                btnSignalRight.BackColor = Color.SandyBrown;
                sendData(rpm, spd, 0, 1, 0);
            }
            else
            {
                btnSignalRight.BackColor = SystemColors.Control;
                sendData(rpm, spd, 0, 2, 0);
            }
        }

        private void tbSpeed_Scroll(object sender, EventArgs e)
        {
            spd = tbSpeed.Value;
            if (comPort.IsOpen) sendData(rpm, spd, 0, 0, 0);
        }

        private void tbRPM_Scroll(object sender, EventArgs e)
        {
            rpm = tbRPM.Value;
            if (comPort.IsOpen) sendData(rpm, spd, 0, 0, 0);
        }

    }
}
