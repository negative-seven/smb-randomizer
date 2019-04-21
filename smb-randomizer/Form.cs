using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace smb_randomizer
{
	public partial class Form : System.Windows.Forms.Form
	{
		public Form()
		{
			InitializeComponent();
		}

		private void RandomSeedCheckBox_Changed(object sender, EventArgs e)
		{
			SeedTextBox.Enabled = !RandomSeedCheckBox.Checked;
		}

		private void SeedTextBox_TextChanged(object sender, EventArgs e)
		{
			SeedTextBox.Text = Regex.Replace(SeedTextBox.Text, "[^0-9]", ""); // remove non-numeric characters
			
			if (ulong.TryParse(SeedTextBox.Text, out ulong seed) && seed > uint.MaxValue) // set to max value if above max value.
			{
				SeedTextBox.Text = uint.MaxValue.ToString();
			}
		}

		private void RandomizeButton_Clicked(object sender, EventArgs e)
		{
			GameRandomizer randomizer = null;
			try
			{
				randomizer = new GameRandomizer();
			}
			catch (Exception)
			{
				MessageBox.Show("Super Meat Boy window not found.\nTry again with the game open.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return;
			}

			int runSeed;
			if (RandomSeedCheckBox.Checked || !uint.TryParse(SeedTextBox.Text, out uint unsignedRunSeed))
			{
				RandomSeedCheckBox.Checked = true;
				runSeed = (int)DateTime.Now.Ticks;
			}
			else
			{
				runSeed = (int)unsignedRunSeed;
			}
			var random = new Random(runSeed);

			try
			{
				randomizer.DisableLeaderboards();
				randomizer.RandomizeLevels(random.Next());
			}
			catch (Exception)
			{
				MessageBox.Show("Something went wrong during randomization.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				return;
			}

			MessageBox.Show("Randomization finished.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}
	}
}
