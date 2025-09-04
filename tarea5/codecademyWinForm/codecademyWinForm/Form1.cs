using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace codecademyWinForm
{   
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            CargarCursos();
        }

        private void CargarCursos()
        {
            var cursos = new List<Course>();

            var c1 = new Course(); c1.setName("Learn C++: Pointers & Memory"); c1.setDifficulty(3); c1.setDescription("Deep dive into pointers, memory layout, and RAII."); cursos.Add(c1);
            var c2 = new Course(); c2.setName("Python Fundamentals"); c2.setDifficulty(1); c2.setDescription("Variables, control flow, functions, and modules."); cursos.Add(c2);
            var c3 = new Course(); c3.setName("JavaScript Async Patterns"); c3.setDifficulty(2); c3.setDescription("Promises, async/await, and event loops."); cursos.Add(c3);
            var c4 = new Course(); c4.setName("SQL for Data Analysis"); c4.setDifficulty(2); c4.setDescription("SELECTs, JOINs, aggregations, and window functions."); cursos.Add(c4);
            var c5 = new Course(); c5.setName("Advanced Data Structures in C++"); c5.setDifficulty(3); c5.setDescription("Trees, graphs, and performance trade-offs."); cursos.Add(c5);
            var c6 = new Course(); c6.setName("Web APIs with FastAPI"); c6.setDifficulty(2); c6.setDescription("FastAPI endpoints, Pydantic models, and auth."); cursos.Add(c6);

            applyfilters(cursos);
        }

        private void applyfilters(List<Course> courses)
        {
            var filteredCourses = new List<Course>();
            bool begginer = checkBox1.Checked;
            bool intermediate = checkBox2.Checked;
            bool advanced = checkBox3.Checked;

            for (int i = 0; i < courses.Count; i++)
            {
                if (!begginer && !intermediate && !advanced)
                {
                    filteredCourses = courses;
                    break;
                }
                if (begginer && courses[i].getDifficulty() == 1)
                {
                    filteredCourses.Add(courses[i]);
                    continue;
                }
                if (intermediate && courses[i].getDifficulty() == 2)
                {
                    filteredCourses.Add(courses[i]);
                    continue;
                }
                if (advanced && courses[i].getDifficulty() == 3)
                {
                    filteredCourses.Add(courses[i]);
                    continue;
                }
            }
            listBox1.DataSource = filteredCourses;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // Boton apply filters
            CargarCursos();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {   

        }

        private void AbrirCursoSeleccionado()
        {
            if (listBox1.SelectedItem is Course selected)
            {
                var detalle = new Form2(selected);

                detalle.ShowDialog(this);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // Boton Open Course
            AbrirCursoSeleccionado();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }
    }


    public class Course
    {
        private string name;
        private int level; // Begginer: 1, Intermediate: 2, Advanced: 3
        private string description;
        public void setName (string name)
        {
            this.name = name;
        }
        public void setDifficulty(int level)
        {
            this.level = level;
        }
        public void setDescription(string description)
        {
            this.description = description;
        }
        public string getName()
        {
            return this.name;
        }
        public int getDifficulty()
        {
            return this.level;
        }
        public string getDescription()
        {
            return this.description;
        }
        public override string ToString()
        => $"{name} {(level == 1 ? "[Beginner]" : level == 2 ? "[Intermediate]" : "[Advanced]")}";
    }

}
