﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace Urho3DGenerator
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start("http://www.urho3d.com");
		}

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void btnGen_Click(object sender, EventArgs e)
        {
            if (EnvCheckHelper.IsPathExistExe("cmake.exe") == false)
            {
                MessageBox.Show("请正确安装cmake，并将cmake.exe路径放入path环境变量！");
                return;
            }

            //1. command
			string batName = get_command_string();

            //2. param
			string strParams = get_param_string();

            Process pro = Process.Start(batName, strParams);
        }

		private string get_command_string()
		{
			if (radVS2008.Checked == true)
			{
				return "cmake_vs2008.bat";
			}
			else if (radVS2010.Checked == true)
			{
				return "cmake_vs2010.bat";
			}
			else if (radVS2012.Checked == true)
			{
				return "cmake_vs2012.bat";
			}
			else if (radVS2013.Checked == true)
			{
				return "cmake_vs2013.bat";
			}
			else
			{
				MessageBox.Show("非法的开发平台！");
				return "";
			}
		}

		private string get_param_string()
		{
			string renderer = "";
			if (radOpenGL2.Checked == true)
			{
				renderer = "-DURHO3D_OPENGL=1";
			}

			string strOption = "";
			if (chkLua.Checked == true)
			{
				strOption += " -DURHO3D_LUA=1";
			}
			if (chkLuaJit.Checked == true)
			{
				strOption += " -DURHO3D_LUAJIT=1";
			}
			if (chkSamples.Checked == true)
			{
				strOption += " -DURHO3D_SAMPLES=1";
			}
			if (chkAngleScript.Checked == true)
			{
				strOption += " -DURHO3D_ANGELSCRIPT=1";
			}

			//4.Lib Type
			string strLibType = ""; //默认
			if (radDllBuild.Checked == true)
			{
				strLibType += " -DURHO3D_LIB_TYPE=SHARED";
			}
			if (radStaticLib.Checked == true)
			{
				strLibType += " -DURHO3D_LIB_TYPE=STATIC";
			}

			string strParams = renderer + strOption + strLibType;

			return strParams;
		}

        private void btnClean_Click(object sender, EventArgs e)
        {
            Process pro = Process.Start("cmake_clean.bat", "");
        }

        private void chkLua_CheckedChanged(object sender, EventArgs e)
        {
            if (chkLua.Checked == false)
            {
                chkLuaJit.Checked = false;
                chkLuaJit.Enabled = false;
            }
            else
            {
                chkLuaJit.Enabled = true;
            }
        }

		private void lnkRefresh_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			textBox1.Text = get_command_string() + " " + get_param_string();
		}
	}
}
