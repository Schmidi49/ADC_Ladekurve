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
using System.Net;
using System.Net.Sockets;
namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp); //Create UDP Socket

        IPAddress serverAddr = IPAddress.Parse("90.146.19.18"); //Set IP of NodeRed server
        IPAddress[] marcAddr = Dns.GetHostAddresses("home.bolverblitz.net"); //Get and Set IP of JS Webserver

        IPEndPoint endPoint; //Create NodeRed Endpoint
        IPEndPoint marcpoint; //Create JS Webserver Endpoint
        

        public Form1()
        {
            InitializeComponent();
            panelStatus.BackColor = Color.Red;
            endPoint = new IPEndPoint(serverAddr, 1337); //Set NodeRed Endpoint
            marcpoint = new IPEndPoint(marcAddr[0], 1024); //Set JS Webserver Endpoint
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.DataReceived += SerialPort1_DataReceived; //Event for Serial Data recive
        }

        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e) //Data Recive Event
        {
            string data = serialPort1.ReadLine().Trim(); //Read and Save data 
         
            byte[] send_buffer = Encoding.ASCII.GetBytes(data); //Create ASCII encoded Data for UDP
            sock.SendTo(send_buffer, endPoint); //Send Data to NodeRED Server
            sock.SendTo(send_buffer, marcpoint); //Send Data to JS Webserver
        }

        //search all available Ports on Syttem
        private void ListCom()
        {
            //Write all available ports into String
            string[] ports = SerialPort.GetPortNames();

            //Display available Ports
            foreach(string port in ports)
            {
                cboPorts.Items.Add(port);
            }
                 
        }

        private void cboPorts_Click(object sender, EventArgs e)
        {
            ListCom(); //List Ports in Menu
        }

        //Open Serial Port
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(cboPorts.SelectedIndex == -1)
            {
                MessageBox.Show("Please select COM-POrt");
            }
            else
            {
                serialPort1.PortName = cboPorts.SelectedItem.ToString();
                if(!serialPort1.IsOpen)
                {
                    try
                    {
                        serialPort1.Open();
                        MessageBox.Show("Port opened");
                        panelStatus.BackColor = Color.Green;
                    }
                    catch(Exception ex)
                    {
                        MessageBox.Show("A error occurred!");
                        MessageBox.Show(ex.ToString());
                        panelStatus.BackColor = Color.Yellow;
                    }
                }
            }
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen)
            {
                serialPort1.Close();
                MessageBox.Show("Port closed");
                panelStatus.BackColor = Color.Red;
            }
            else
            {
                MessageBox.Show("Port not open");
            }
        }

        private void closeApplicationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
