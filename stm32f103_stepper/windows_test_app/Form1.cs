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

namespace stepper
{
    public partial class Form1 : Form
    {

        private SerialPort comPort = new SerialPort();

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

        private void btnTransistor_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            if (btnTransistor.BackColor == SystemColors.Control)
            {
                btnTransistor.BackColor = Color.SpringGreen;
                btnTransistor.Text = "Transistor Off";
                try
                {
                    comPort.Write("t");
                    comPort.Write("1");
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            else
            {
                btnTransistor.BackColor = SystemColors.Control;
                btnTransistor.Text = "Transistor On";
                try
                {
                    comPort.Write("t");
                    comPort.Write("0");
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            
        }

        private void btnRelay_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            if (btnRelay.BackColor == SystemColors.Control)
            {
                btnRelay.BackColor = Color.SpringGreen;
                btnRelay.Text = "Relay Off";
                try
                {
                    comPort.Write("r");
                    comPort.Write("1");
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            else
            {
                btnRelay.BackColor = SystemColors.Control;
                btnRelay.Text = "Relay On";
                try
                {
                    comPort.Write("r");
                    comPort.Write("0");
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void btnMot1_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            int steps = int.Parse(tbMot1.Text);
            if (steps > 9999 || steps < -9999) return;
            comPort.Write("m");
            comPort.Write("1");
            if (steps < 0)
            {
                comPort.Write("-");
                steps = -steps;
            }
            comPort.Write((steps/1000).ToString());
            comPort.Write(((steps / 100) % 10).ToString());
            comPort.Write(((steps / 10) % 10).ToString());
            comPort.Write((steps % 10).ToString());
        }

        private void btnMot2_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            int steps = int.Parse(tbMot2.Text);
            if (steps > 9999 || steps < -9999) return;
            comPort.Write("m");
            comPort.Write("2");
            if (steps < 0)
            {
                comPort.Write("-");
                steps = -steps;
            }
            comPort.Write((steps / 1000).ToString());
            comPort.Write(((steps / 100) % 10).ToString());
            comPort.Write(((steps / 10) % 10).ToString());
            comPort.Write((steps % 10).ToString());
        }

        private void btnMot3_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            int steps = int.Parse(tbMot3.Text);
            if (steps > 9999 || steps < -9999) return;
            comPort.Write("m");
            comPort.Write("3");
            if (steps < 0)
            {
                comPort.Write("-");
                steps = -steps;
            }
            comPort.Write((steps / 1000).ToString());
            comPort.Write(((steps / 100) % 10).ToString());
            comPort.Write(((steps / 10) % 10).ToString());
            comPort.Write((steps % 10).ToString());
        }

        private void cbDriveMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!comPort.IsOpen) return;
            comPort.Write("d");
            comPort.Write(cbDriveMode.SelectedIndex.ToString());
        }

    }
}
