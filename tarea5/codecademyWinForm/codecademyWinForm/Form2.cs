using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace codecademyWinForm
{
    public partial class Form2 : Form
    {
        public Form2(Course course)
        {
            InitializeComponent();
            this.Text = course.getName(); // título del form
            label1.Text = course.getName();
            label3.Text = $"Level: {course.getDifficulty()}";
            label2.Text = course.getDescription();
        }


        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
