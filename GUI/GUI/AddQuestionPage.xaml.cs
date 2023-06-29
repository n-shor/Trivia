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

        if (!(correctAnswer != "a" && correctAnswer != "b" && correctAnswer != "c" && correctAnswer != "d"))
        {
            var l = new AddQuestionRequest { correctAnswer = correctAnswer[0], optionA = opA, optionB = opB, optionC = opC, optionD = opD, question = question };
            string jsonString = JsonSerializer.Serialize(l);

            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.AddQuestion, jsonString);

            var data = Deserializer.getResponse(ClientSocket.sock);
            AddQuestionResponse json = JsonSerializer.Deserialize<AddQuestionResponse>(data.jsonData);

            if (json.status == (int)MenuRequestStatus.QuestionAdded)
            {
                DisplayAlert("Added question", "Your question has been added to the database!", "OK");
                Navigation.PushAsync(new MainMenuPage());
            }
            else
            {
                DisplayAlert("Add Question Failed", "There was an error adding your question to the database. Please try again later.", "OK");
            }
        }
        else
        {
            DisplayAlert("Add Question Failed", "The 'Correct Answer' field contains an invalid value.", "OK");
        }
    }
    private async void OnBackButtonClicked(object sender, EventArgs e)
    {
        await Navigation.PushAsync(new MainMenuPage());
    }
}