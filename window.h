#pragma once

#include <msclr\marshal_cppstd.h>

#include "randomize.h"
#include "randomize_game.h"

namespace SuperMeatBoyrandomizer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for window
	/// </summary>
	public ref class window : public System::Windows::Forms::Form
	{
	System::String^ input_seed_backup;

	public:
		window(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~window()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button_randomize;
	protected:

	protected:

	private: System::Windows::Forms::TextBox^  input_seed;
	private: System::Windows::Forms::CheckBox^  input_useRandom;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(window::typeid));
			this->button_randomize = (gcnew System::Windows::Forms::Button());
			this->input_seed = (gcnew System::Windows::Forms::TextBox());
			this->input_useRandom = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// button_randomize
			// 
			this->button_randomize->Location = System::Drawing::Point(12, 38);
			this->button_randomize->Name = L"button_randomize";
			this->button_randomize->Size = System::Drawing::Size(185, 23);
			this->button_randomize->TabIndex = 0;
			this->button_randomize->Text = L"Randomize";
			this->button_randomize->UseVisualStyleBackColor = true;
			this->button_randomize->Click += gcnew System::EventHandler(this, &window::button_randomize_Click);
			// 
			// input_seed
			// 
			this->input_seed->CharacterCasing = System::Windows::Forms::CharacterCasing::Lower;
			this->input_seed->Location = System::Drawing::Point(12, 12);
			this->input_seed->MaxLength = 10;
			this->input_seed->Name = L"input_seed";
			this->input_seed->ReadOnly = true;
			this->input_seed->Size = System::Drawing::Size(70, 20);
			this->input_seed->TabIndex = 1;
			this->input_seed->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->input_seed->TextChanged += gcnew System::EventHandler(this, &window::input_seed_TextChanged);
			// 
			// input_useRandom
			// 
			this->input_useRandom->AutoSize = true;
			this->input_useRandom->Checked = true;
			this->input_useRandom->CheckState = System::Windows::Forms::CheckState::Checked;
			this->input_useRandom->Location = System::Drawing::Point(88, 14);
			this->input_useRandom->Name = L"input_useRandom";
			this->input_useRandom->Size = System::Drawing::Size(109, 17);
			this->input_useRandom->TabIndex = 2;
			this->input_useRandom->Text = L"Use random seed";
			this->input_useRandom->UseVisualStyleBackColor = true;
			this->input_useRandom->CheckedChanged += gcnew System::EventHandler(this, &window::input_useRandom_CheckedChanged);
			// 
			// window
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(209, 73);
			this->Controls->Add(this->input_useRandom);
			this->Controls->Add(this->input_seed);
			this->Controls->Add(this->button_randomize);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"window";
			this->ShowIcon = false;
			this->Text = L"Super Meat Boy Randomizer";
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	private:
		System::Void input_useRandom_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		{
			input_seed->ReadOnly = input_useRandom->Checked;

			if (input_useRandom->Checked)
			{
				input_seed->Text = "";
			}
			else
			{
				input_seed->Text = input_seed_backup;
			}
		}
	private:
		System::Void input_seed_TextChanged(System::Object^  sender, System::EventArgs^  e)
		{
			// Remove any non-numerical character.

			for (int i = input_seed->Text->Length - 1; i >= 0; i--)
			{
				if (input_seed->Text[i] < '0' || input_seed->Text[i] > '9')
				{
					input_seed->Text = input_seed->Text->Remove(i, 1);
				}
			}

			// Set to max value if above max value.

			if (input_seed->Text != L"")
			{
				if (Convert::ToInt64(input_seed->Text) > UINT_MAX)
				{
					input_seed->Text = Convert::ToString(UINT_MAX);
				}
			}

			if (!input_useRandom->Checked)
			{
				input_seed_backup = input_seed->Text;
			}
		}
	private:
		System::Void button_randomize_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if (input_useRandom->Checked)
			{
				randomize_game(randomInt());
			}
			else
			{
				randomize_game((unsigned int)Convert::ToInt64(input_seed->Text)); // Converts to unsigned int
			}
		}
	};
}