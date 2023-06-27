using System.Text.Json;

namespace GUI;

public partial class AddQuestionPage : ContentPage
{
	public AddQuestionPage()
	{
		InitializeComponent();
	}

    private void AddQuestionButtonClicked(object sender, EventArgs e)
    {

        string question = QuestionEntry.Text;
        string opA = OptionAEntry.Text;
        string opB = OptionBEntry.Text;
        string opC = OptionCEntry.Text;
        string opD = OptionDEntry.Text;
        string correctAnswer = AnswerEntry.Text;

        if (string.IsNullOrEmpty(question) || string.IsNullOrEmpty(opA) || string.IsNullOrEmpty(opB) || string.IsNullOrEmpty(opC) || string.IsNullOrEmpty(opD) || string.IsNullOrEmpty(correctAnswer))
        {
            DisplayAlert("Add Question Failed", "Please fill out all of the relevant fields.", "OK");
            return;
        }

        if (correctAnswer.Length == 1)
        {
            var l = new AddQuestionRequest { correctAnswer = correctAnswer[0], optionA = opA, optionB = opB, optionC = opC, optionD = opD, question = question };
            string jsonString = JsonSerializer.Serialize(l);

            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.AddQuestion, jsonString);

            var data = Deserielizer.getResponse(ClientSocket.sock);
            AddQuestionResponse json = JsonSerializer.Deserialize<AddQuestionResponse>(data.jsonData);

            // Now we check if the registration was successful.
            if (json.status == (int)MenuRequestStatus.QuestionAdded)
            {
                DisplayAlert("Added question", "Your question has been added", "OK");
                Navigation.PushAsync(new MainMenuPage());
            }
            else
            {
                // If registration failed (user already exists), display an alert
                DisplayAlert("Add Question Failed", "Your question was invalid", "OK");
            }
        }
        else
        {
            // If email is invalid, display an alert
            DisplayAlert("Add Question Failed", "Invalid answer", "OK");
        }
    }
    private async void OnBackButtonClicked(object sender, EventArgs e)
    {
        await Navigation.PushAsync(new MainMenuPage());
    }
}