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

namespace usbio
{
    public partial class Form1 : Form
    {

        private SerialPort comPort = new SerialPort();
        private int pot = 0;

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

        private void btnSend_Click(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            try
            {
                string str = tbLine1.Text;
                for (int i = str.Length; i < 16; i++) str += " ";
                str += tbLine2.Text;
                for (int i = str.Length; i < 32; i++) str += " ";
                comPort.Write("$");
                for (int i = 0; i < 32; i++)
                {
                    byte[] c = new byte[1];
                    switch (str[i])
                    {
                        case 'á': c[0] = 0x01; break;
                        case 'é': c[0] = 0x02; break;
                        case 'í': c[0] = 0x03; break;
                        case 'ó': c[0] = 0x04; break;
                        case 'ö': c[0] = 0x05; break;
                        case 'ő': c[0] = 0x06; break;
                        case 'ú': c[0] = 0x07; break;
                        default: c[0] = (byte)str[i]; break;
                    }
                    comPort.Write(c, 0, 1);
                }
                comPort.Write("|");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            timer1.Enabled = true;
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
                timer1.Enabled = true;
            }
            else
            {
                timer1.Enabled = false;
                if (comPort.IsOpen) comPort.Close();
                btnConnect.BackColor = SystemColors.Control;
                btnConnect.Text = "Connect";
            }
        }

        private void updatePot(TrackBar tb, int n)
        {
            if ((n < 0) || (n > 9)) return;
            comPort.Write(n.ToString());
            string reply = comPort.ReadTo("\n");
            tb.Value = Int32.Parse(reply);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            switch (pot)
            {
                case 0:  updatePot(tb0, 0); break;
                case 1:  updatePot(tb1, 1); break;
                case 2:  updatePot(tb2, 2); break;
                case 3:  updatePot(tb3, 3); break;
                case 4:  updatePot(tb4, 4); break;
                case 5:  updatePot(tb5, 5); break;
                case 6:  updatePot(tb6, 6); break;
                case 7:  updatePot(tb7, 7); break;
                case 8:  updatePot(tb8, 8); break;
                case 9:  updatePot(tb9, 9); break;
                default: break;
            }
            pot = (pot + 1) % 10;
        }

    }
}
