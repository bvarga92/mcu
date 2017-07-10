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
using System.Threading;

namespace agilentlogger
{
    public partial class Form1 : Form
    {

        private SerialPort comPort = new SerialPort();
        private double[] buf = new double[100];
        private int bufEnd = -1;
        private int bufValidCnt = 0;
        private Bitmap bm;
        private Graphics g;
        private System.IO.StreamWriter file;

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
            bm = new Bitmap(pictureBox.ClientRectangle.Width, pictureBox.ClientRectangle.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            g = Graphics.FromImage(bm);
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
                comPort.DataReceived += new SerialDataReceivedEventHandler(dataReceivedHandler);
                try
                {
                    comPort.Open();
                    file = new System.IO.StreamWriter("./agilent_log.txt");
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
                if (comPort.IsOpen) (new Thread(() => { comPort.Close(); })).Start(); //bugfix
                btnConnect.BackColor = SystemColors.Control;
                btnConnect.Text = "Connect";
                file.Close();
            }
        }

        private void dataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string str = ((SerialPort)sender).ReadTo("\n");
                double d;
                Double.TryParse(str, System.Globalization.NumberStyles.Float, System.Globalization.NumberFormatInfo.InvariantInfo, out d);
                bufEnd = (bufEnd + 1) % buf.Length;
                buf[bufEnd] = d;
                if (bufValidCnt < buf.Length) bufValidCnt++;
                file.WriteLine(str);
                BeginInvoke(new Action(() => { plotData(); }));
            }
            catch (Exception)
            {

            }
        }

        private void plotData()
        {
            double max=buf[0];
            for (int i = buf.Length - bufValidCnt; i < buf.Length; i++)
            {
                double val = Math.Abs(buf[(bufEnd + 1 + i) % buf.Length]);
                if (val > max) max = val;
            }
            double scale = (max == 0) ? 1 : (100 / max);
            g.Clear(SystemColors.Control);
            int x = (buf.Length - bufValidCnt) * 3;
            double yPrev = 0;
            for (int i = buf.Length - bufValidCnt; i < buf.Length; i++, x += 3)
            {
                double y = buf[(bufEnd + 1 + i) % buf.Length];
                if (i > buf.Length - bufValidCnt) g.DrawLine(new Pen(System.Drawing.Color.Red, 2), x - 3, 100 - (int)(yPrev * scale), x, 100 - (int)(y * scale));
                yPrev = y;
            }
            pictureBox.Image = bm;
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < buf.Length; i++) buf[i] = 0;
            bufValidCnt = 0;
            bufEnd=0;
            if (comPort.IsOpen) file.WriteLine("-------------------------------------------------------------");
        }

    }
}
