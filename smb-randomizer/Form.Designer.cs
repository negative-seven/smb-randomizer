namespace smb_randomizer
{
	partial class Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form));
			this.SeedTextBox = new System.Windows.Forms.TextBox();
			this.RandomSeedCheckBox = new System.Windows.Forms.CheckBox();
			this.RandomizeButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// SeedTextBox
			// 
			this.SeedTextBox.Enabled = false;
			this.SeedTextBox.Location = new System.Drawing.Point(12, 12);
			this.SeedTextBox.MaxLength = 10;
			this.SeedTextBox.Name = "SeedTextBox";
			this.SeedTextBox.Size = new System.Drawing.Size(100, 20);
			this.SeedTextBox.TabIndex = 0;
			this.SeedTextBox.TextChanged += new System.EventHandler(this.SeedTextBox_TextChanged);
			// 
			// RandomSeedCheckBox
			// 
			this.RandomSeedCheckBox.AutoSize = true;
			this.RandomSeedCheckBox.Checked = true;
			this.RandomSeedCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
			this.RandomSeedCheckBox.Location = new System.Drawing.Point(118, 14);
			this.RandomSeedCheckBox.Name = "RandomSeedCheckBox";
			this.RandomSeedCheckBox.Size = new System.Drawing.Size(109, 17);
			this.RandomSeedCheckBox.TabIndex = 1;
			this.RandomSeedCheckBox.Text = "Use random seed";
			this.RandomSeedCheckBox.UseVisualStyleBackColor = true;
			this.RandomSeedCheckBox.CheckedChanged += new System.EventHandler(this.RandomSeedCheckBox_Changed);
			// 
			// RandomizeButton
			// 
			this.RandomizeButton.Location = new System.Drawing.Point(12, 38);
			this.RandomizeButton.Name = "RandomizeButton";
			this.RandomizeButton.Size = new System.Drawing.Size(208, 23);
			this.RandomizeButton.TabIndex = 2;
			this.RandomizeButton.Text = "Randomize";
			this.RandomizeButton.UseVisualStyleBackColor = true;
			this.RandomizeButton.Click += new System.EventHandler(this.RandomizeButton_Clicked);
			// 
			// Form
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(232, 73);
			this.Controls.Add(this.RandomizeButton);
			this.Controls.Add(this.RandomSeedCheckBox);
			this.Controls.Add(this.SeedTextBox);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Form";
			this.Text = "SMB randomizer";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox SeedTextBox;
		private System.Windows.Forms.CheckBox RandomSeedCheckBox;
		private System.Windows.Forms.Button RandomizeButton;
	}
}

