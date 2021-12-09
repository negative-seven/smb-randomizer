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
			this.RandomizeButton = new System.Windows.Forms.Button();
			this.RandomSeedButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// SeedTextBox
			// 
			this.SeedTextBox.Location = new System.Drawing.Point(12, 12);
			this.SeedTextBox.MaxLength = 10;
			this.SeedTextBox.Name = "SeedTextBox";
			this.SeedTextBox.Size = new System.Drawing.Size(100, 20);
			this.SeedTextBox.TabIndex = 0;
			this.SeedTextBox.TextChanged += new System.EventHandler(this.SeedTextBox_TextChanged);
			// 
			// RandomizeButton
			// 
			this.RandomizeButton.Location = new System.Drawing.Point(11, 38);
			this.RandomizeButton.Name = "RandomizeButton";
			this.RandomizeButton.Size = new System.Drawing.Size(230, 23);
			this.RandomizeButton.TabIndex = 2;
			this.RandomizeButton.Text = "Randomize";
			this.RandomizeButton.UseVisualStyleBackColor = true;
			this.RandomizeButton.Click += new System.EventHandler(this.RandomizeButton_Clicked);
			// 
			// RandomSeedButton
			// 
			this.RandomSeedButton.Location = new System.Drawing.Point(118, 10);
			this.RandomSeedButton.Name = "RandomSeedButton";
			this.RandomSeedButton.Size = new System.Drawing.Size(123, 23);
			this.RandomSeedButton.TabIndex = 3;
			this.RandomSeedButton.Text = "Generate random seed";
			this.RandomSeedButton.UseVisualStyleBackColor = true;
			this.RandomSeedButton.Click += new System.EventHandler(this.RandomSeedButton_Click);
			// 
			// Form
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(252, 73);
			this.Controls.Add(this.RandomSeedButton);
			this.Controls.Add(this.RandomizeButton);
			this.Controls.Add(this.SeedTextBox);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Form";
			this.Text = "SMB randomizer";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox SeedTextBox;
		private System.Windows.Forms.Button RandomizeButton;
		private System.Windows.Forms.Button RandomSeedButton;
	}
}

