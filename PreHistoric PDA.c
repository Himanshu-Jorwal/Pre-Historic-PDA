#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define Max_Dinosaurs 100                       // Dinosaur PDA 
#define Len_Name 50
#define Len_Era 30
#define Len_Diet 20
#define Len_Social 20
#define Len_Habitat 50
#define Len_Fossil 50
#define Len_Info 200

#define Max_Trivia 20                           // Dinosaur Trivia
#define Len_Question 100
#define Len_Answer 50
#define Max_Options 4

typedef struct {
    char Name[Len_Name];
    char Era[Len_Era];
    float Height;
    float Length;
    float Weight;
    char Diet[Len_Diet];
    char Social[Len_Social];
    char Habitat[Len_Habitat];
    char Fossil[Len_Fossil];
    char Info[Len_Info];
} PDA;

typedef struct {
    char Question[Len_Question];
    char Options[Max_Options][Len_Answer];
    int CorrectAnswerIndex;
} Trivia;

PDA Dinosaurs[Max_Dinosaurs];
int DinosaurCount = 0;

Trivia Quiz[Max_Trivia];
int QuizCount = 0;
int HighScore = 0;          // To Track HighScore
int LongestStreak = 0;      // To Track Streak Count

void LoadDinosaurInfo();
void SaveDinosaurInfo();
void ShowDinosaur(PDA SingleDino);
void SearchDinosaur();
void SearchByEra();
void AddDinosaur();
void LoadTrivia();
void StartTrivia();
void StartScoreMode();
void StartKnockoutMode();
void UpdateHighScore(int Score, int Streak);

int main() {
    srand(time(NULL));                  // Seed The RNG

    LoadDinosaurInfo();               // Load PDA
    LoadTrivia();                       // Load Trivia

    int Task;
    do {
        printf("\nDinosaur PDA\n");
        printf("1) Search by Dinosaur's Name\n");
        printf("2) Search by Era\n");
        printf("3) Help Us Improve\n");
        printf("4) Take a Trivia Quiz\n");
        printf("5) Exit\n");
        printf("Enter your Task : ");
        scanf("%d", &Task);
        getchar();

        // Selecting which Task to perform
        switch (Task) {
            case 1:
                SearchDinosaur();
                break;
            case 2:
                SearchByEra();
                break;
            case 3:
                AddDinosaur();
                break;
            case 4:
                StartTrivia();
                break;
            case 5:
                printf("Affirmative! Exiting the Program.\n");
                break;
            default:
                printf("Invalid Task! Try Again.\n");
        }
    } while (Task != 5);

    return 0;
}

// Accessing the txt File with Dinosaur Info
void LoadDinosaurInfo() {
    FILE *File = fopen("PDA_Info.txt", "r");
    if (File == NULL) {
        printf("No PDA Info file found. Initializing with Default Data.\n");
        return;
    }

    // Reset count before loading
    DinosaurCount = 0; 
    while (DinosaurCount < Max_Dinosaurs && fscanf(File, "%[^,],%[^,],%f,%f,%f,%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
        Dinosaurs[DinosaurCount].Name,
        Dinosaurs[DinosaurCount].Era,
        &Dinosaurs[DinosaurCount].Height,
        &Dinosaurs[DinosaurCount].Length,
        &Dinosaurs[DinosaurCount].Weight,
        Dinosaurs[DinosaurCount].Diet,
        Dinosaurs[DinosaurCount].Social,
        Dinosaurs[DinosaurCount].Habitat,
        Dinosaurs[DinosaurCount].Fossil,
        Dinosaurs[DinosaurCount].Info) == 10) {
        DinosaurCount++;
    }
    
    fclose(File);
    printf("Loaded %d Dinosaurs from the File.\n", DinosaurCount);
}

// Format of Displaying Information
void ShowDinosaur(PDA SingleDino) {
    printf("Name             : %s\n", SingleDino.Name);
    printf("Era              : %s\n", SingleDino.Era);
    printf("Height           : %.2f m\n", SingleDino.Height);
    printf("Length           : %.2f m\n", SingleDino.Length);
    printf("Weight           : %.2f kg\n", SingleDino.Weight);
    printf("Diet             : %s\n", SingleDino.Diet);
    printf("Social Behavior  : %s\n", SingleDino.Social);
    printf("Habitat          : %s\n", SingleDino.Habitat);
    printf("Fossil Locations : %s\n", SingleDino.Fossil);
    printf("Info             : %s\n", SingleDino.Info);
}

// Looking up Dinosaur with their Name or Initials
void SearchDinosaur() {
    char Name[Len_Name];
    printf("Enter Dinosaur's Name : ");
    fgets(Name, sizeof(Name), stdin);
    Name[strcspn(Name, "\n")] = 0;

    for (int i = 0; i < DinosaurCount; i++) {
        if (strncasecmp(Dinosaurs[i].Name, Name, strlen(Name)) == 0) {
            ShowDinosaur(Dinosaurs[i]);
            return;
        }
    }
    printf("Negative! Dinosaur Not Found.\n");
}

// Looking up which Dinosaurs lived in an Era
void SearchByEra() {
    char Era[Len_Era];
    printf("Enter the Era : ");
    fgets(Era, sizeof(Era), stdin);
    Era[strcspn(Era, "\n")] = 0;

    int Found = 0;
    printf("Dinosaurs that lived in the %s Era : \n", Era);
    for (int i = 0; i < DinosaurCount; i++) {
        if (strcasecmp(Dinosaurs[i].Era, Era) == 0) {
            printf("- %s\n", Dinosaurs[i].Name);
            Found = 1;
        }
    }
    if (!Found) {
        printf("None.\n");
    }
}

// Adding New Dinosaur to PDA
void AddDinosaur() {
    PDA NewDino;

    printf("Enter Dinosaur's Name : ");
    fgets(NewDino.Name, sizeof(NewDino.Name), stdin);
    NewDino.Name[strcspn(NewDino.Name, "\n")] = 0;
    if (strlen(NewDino.Name) == 0) {
        printf("Error! Name cannot be empty.\n");
        return;
    }

    printf("Enter Era : ");
    fgets(NewDino.Era, sizeof(NewDino.Era), stdin);
    NewDino.Era[strcspn(NewDino.Era, "\n")] = 0;
    if (strlen(NewDino.Era) == 0) {
        printf("Error! Era cannot be empty.\n");
        return;
    }

    printf("Enter Height (M): ");
    if (scanf("%f", &NewDino.Height) != 1 || NewDino.Height <= 0) {
        printf("Error! Height must be a positive number.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Length (M): ");
    if (scanf("%f", &NewDino.Length) != 1 || NewDino.Length <= 0) {
        printf("Error! Length must be a positive number.\n");
        while (getchar() != '\n'); 
        return;
    }

    printf("Enter Weight (Kg): ");
    if (scanf("%f", &NewDino.Weight) != 1 || NewDino.Weight <= 0) {
        printf("Error! Weight must be a positive number.\n");
        while (getchar() != '\n'); 
        return;
    }
    getchar();

    printf("Enter Diet : ");
    fgets(NewDino.Diet, sizeof(NewDino.Diet), stdin);
    NewDino.Diet[strcspn(NewDino.Diet, "\n")] = 0;
    if (strlen(NewDino.Diet) == 0) {
        printf("Error! Diet cannot be empty.\n");
        return;
    }

    printf("Enter Social Behavior : ");
    fgets(NewDino.Social, sizeof(NewDino.Social), stdin);
    NewDino.Social[strcspn(NewDino.Social, "\n")] = 0;
    if (strlen(NewDino.Social) == 0) {
        printf("Error! Social Behavior cannot be empty.\n");
        return;
    }

    printf("Enter Habitat : ");
    fgets(NewDino.Habitat, sizeof(NewDino.Habitat), stdin);
    NewDino.Habitat[strcspn(NewDino.Habitat, "\n")] = 0;
    if (strlen(NewDino.Habitat) == 0) {
        printf("Error! Habitat cannot be empty.\n");
        return;
    }

    printf("Enter Fossil Locations : ");
    fgets(NewDino.Fossil, sizeof(NewDino.Fossil), stdin);
    NewDino.Fossil[strcspn(NewDino.Fossil, "\n")] = 0;
    if (strlen(NewDino.Fossil) == 0) {
        printf("Error! Fossil Locations cannot be empty.\n");
        return;
    }

    printf("Enter Info : ");
    fgets(NewDino.Info, sizeof(NewDino.Info), stdin);
    NewDino.Info[strcspn(NewDino.Info, "\n")] = 0;
    if (strlen(NewDino.Info) == 0) {
        printf("Error! Info cannot be empty.\n");
        return;
    }

    // Add the New Dinosaur to the list
    if (DinosaurCount < Max_Dinosaurs) {
        Dinosaurs[DinosaurCount] = NewDino;
        DinosaurCount++;
        printf("Successfully added to PDA.\nTotal Dinosaurs : %d\n", DinosaurCount);
        SaveDinosaurInfo();
    } else {
        printf("Negative! PDA storage is full.\n");
    }
}

void SaveDinosaurInfo() {
    FILE *File = fopen("PDA_Info.txt", "a");
    if (File == NULL) {
        printf("Error! Opening file for Writing.\n");
        return;
    }
    
    PDA NewDino = Dinosaurs[DinosaurCount - 1]; 
    fprintf(File, "%s,%s,%.2f,%.2f,%.2f,%s,%s,%s,%s,%s\n",
            NewDino.Name,
            NewDino.Era,
            NewDino.Height,
            NewDino.Length,
            NewDino.Weight,
            NewDino.Diet,
            NewDino.Social,
            NewDino.Habitat,
            NewDino.Fossil,
            NewDino.Info);
    
    fclose(File);
    printf("Data saved to the PDA.\n");
}

// Trivia Questions List
void LoadTrivia() {
    strcpy(Quiz[0].Question, "Which dinosaur is known as the 'Tyrant Lizard King'?");
    strcpy(Quiz[0].Options[0], "Tyrannosaurus rex");
    strcpy(Quiz[0].Options[1], "Velociraptor");
    strcpy(Quiz[0].Options[2], "Brachiosaurus");
    strcpy(Quiz[0].Options[3], "Stegosaurus");
    Quiz[0].CorrectAnswerIndex = 0;

    strcpy(Quiz[1].Question, "Which dinosaur had a long neck?");
    strcpy(Quiz[1].Options[0], "Majungasaurus");
    strcpy(Quiz[1].Options[1], "Triceratops");
    strcpy(Quiz[1].Options[2], "Brachiosaurus");
    strcpy(Quiz[1].Options[3], "Ankylosaurus");
    Quiz[1].CorrectAnswerIndex = 2;

    strcpy(Quiz[2].Question, "Which dinosaur is famous for its horns?");
    strcpy(Quiz[2].Options[0], "Stegosaurus");
    strcpy(Quiz[2].Options[1], "Triceratops");
    strcpy(Quiz[2].Options[2], "Diplodocus");
    strcpy(Quiz[2].Options[3], "Nodosaurus");
    Quiz[2].CorrectAnswerIndex = 1;

    strcpy(Quiz[3].Question, "What is the largest dinosaur ever discovered?");
    strcpy(Quiz[3].Options[0], "Velociraptor");
    strcpy(Quiz[3].Options[1], "Sinoceratops");
    strcpy(Quiz[3].Options[2], "Stygimoloch");
    strcpy(Quiz[3].Options[3], "Argentinosaurus");
    Quiz[3].CorrectAnswerIndex = 3;

    strcpy(Quiz[4].Question, "Which dinosaur is known for having a sail-like structure on its back?");
    strcpy(Quiz[4].Options[0], "Dreadnoughtus");
    strcpy(Quiz[4].Options[1], "Spinosaurus");
    strcpy(Quiz[4].Options[2], "Parasaurolophus");
    strcpy(Quiz[4].Options[3], "Baryonyx");
    Quiz[4].CorrectAnswerIndex = 1;

    strcpy(Quiz[5].Question, "What period did the Triceratops live in?");
    strcpy(Quiz[5].Options[0], "Jurassic");
    strcpy(Quiz[5].Options[1], "Cretaceous");
    strcpy(Quiz[5].Options[2], "Triassic");
    strcpy(Quiz[5].Options[3], "Devonian");
    Quiz[5].CorrectAnswerIndex = 1;

    strcpy(Quiz[6].Question, "Which dinosaur is often depicted as having feathers?");
    strcpy(Quiz[6].Options[0], "Tyrannosaurus rex");
    strcpy(Quiz[6].Options[1], "Velociraptor");
    strcpy(Quiz[6].Options[2], "Allosaurus");
    strcpy(Quiz[6].Options[3], "Diplodocus");
    Quiz[6].CorrectAnswerIndex = 1;

    strcpy(Quiz[7].Question, "What type of dinosaur is known as a 'sauropod'?");
    strcpy(Quiz[7].Options[0], "Large herbivores");
    strcpy(Quiz[7].Options[1], "Small carnivores");
    strcpy(Quiz[7].Options[2], "Fast-running dinosaurs");
    strcpy(Quiz[7].Options[3], "Flying reptiles");
    Quiz[7].CorrectAnswerIndex = 0;

    strcpy(Quiz[8].Question, "Which dinosaur is considered the fastest?");
    strcpy(Quiz[8].Options[0], "Suchomimus");
    strcpy(Quiz[8].Options[1], "Ceratosaurus");
    strcpy(Quiz[8].Options[2], "Ornithomimus");
    strcpy(Quiz[8].Options[3], "Gallimimus");
    Quiz[8].CorrectAnswerIndex = 3;

    strcpy(Quiz[9].Question, "What was the primary diet of the Brachiosaurus?");
    strcpy(Quiz[9].Options[0], "Herbivore");
    strcpy(Quiz[9].Options[1], "Carnivore");
    strcpy(Quiz[9].Options[2], "Omnivore");
    strcpy(Quiz[9].Options[3], "Insectivore");
    Quiz[9].CorrectAnswerIndex = 0;

    strcpy(Quiz[10].Question, "Which dinosaur had both spikes and plates on it's back?");
    strcpy(Quiz[10].Options[0], "Stegosaurus");
    strcpy(Quiz[10].Options[1], "Sauropelta");
    strcpy(Quiz[10].Options[2], "Kentrosaurus");
    strcpy(Quiz[10].Options[3], "Mamenchiosaurus");
    Quiz[10].CorrectAnswerIndex = 2;

    strcpy(Quiz[11].Question, "What is the name of the first dinosaur to be named?");
    strcpy(Quiz[11].Options[0], "Iguanodon");
    strcpy(Quiz[11].Options[1], "Megalosaurus");
    strcpy(Quiz[11].Options[2], "Tyrannosaurus rex");
    strcpy(Quiz[11].Options[3], "Archaeopteryx");
    Quiz[11].CorrectAnswerIndex = 1;

    strcpy(Quiz[12].Question, "Which dinosaur is known for its elaborate crest on its head?");
    strcpy(Quiz[12].Options[0], "Parasaurolophus");
    strcpy(Quiz[12].Options[1], "Velociraptor");
    strcpy(Quiz[12].Options[2], "Tyrannosaurus rex");
    strcpy(Quiz[12].Options[3], "Spinosaurus");
    Quiz[12].CorrectAnswerIndex = 0;

    strcpy(Quiz[13].Question, "What is the primary habitat of many dinosaurs?");
    strcpy(Quiz[13].Options[0], "Deserts");
    strcpy(Quiz[13].Options[1], "Oceans");
    strcpy(Quiz[13].Options[2], "Forests");
    strcpy(Quiz[13].Options[3], "Mountains");
    Quiz[13].CorrectAnswerIndex = 2;

    strcpy(Quiz[14].Question, "What is the primary feature that distinguishes theropods?");
    strcpy(Quiz[14].Options[0], "They are herbivores");
    strcpy(Quiz[14].Options[1], "They have long necks");
    strcpy(Quiz[14].Options[2], "They walk on two legs");
    strcpy(Quiz[14].Options[3], "They have armored skin");
    Quiz[14].CorrectAnswerIndex = 2;

    strcpy(Quiz[15].Question, "Which dinosaur lived during the Late Jurassic period and was a giant predator?");
    strcpy(Quiz[15].Options[0], "Spinosaurus");
    strcpy(Quiz[15].Options[1], "Allosaurus");
    strcpy(Quiz[15].Options[2], "Tyrannosaurus rex");
    strcpy(Quiz[15].Options[3], "Ceratosaurus");
    Quiz[15].CorrectAnswerIndex = 1;

    strcpy(Quiz[16].Question, "What is the main reason dinosaurs went extinct?");
    strcpy(Quiz[16].Options[0], "Overhunting by humans");
    strcpy(Quiz[16].Options[1], "Climate change");
    strcpy(Quiz[16].Options[2], "Disease");
    strcpy(Quiz[16].Options[3], "Asteroid Impact");
    Quiz[16].CorrectAnswerIndex = 3;

    strcpy(Quiz[17].Question, "Which dinosaur had a distinctive long neck and small head?");
    strcpy(Quiz[17].Options[0], "Iguanodon");
    strcpy(Quiz[17].Options[1], "Diplodocus");
    strcpy(Quiz[17].Options[2], "Triceratops");
    strcpy(Quiz[17].Options[3], "Ceratosaurus");
    Quiz[17].CorrectAnswerIndex = 1;

    strcpy(Quiz[18].Question, "Which dinosaur is often portrayed in films as a fierce predator?");
    strcpy(Quiz[18].Options[0], "Stegosaurus");
    strcpy(Quiz[18].Options[1], "Velociraptor");
    strcpy(Quiz[18].Options[2], "Brachiosaurus");
    strcpy(Quiz[18].Options[3], "Ankylosaurus");
    Quiz[18].CorrectAnswerIndex = 1;

    strcpy(Quiz[19].Question, "What was the primary cause of the diversity among dinosaurs?");
    strcpy(Quiz[19].Options[0], "Climate stability");
    strcpy(Quiz[19].Options[1], "Geographic isolation");
    strcpy(Quiz[19].Options[2], "Migration patterns");
    strcpy(Quiz[19].Options[3], "Food sources");
    Quiz[19].CorrectAnswerIndex = 1;

    QuizCount = 20;
}

// Initialize Trivia
void StartTrivia() {
    int Mode;
    printf("Select Trivia Mode :\n");
    printf("1) Score Mode\n");
    printf("2) Knockout Mode\n");
    printf("Enter your Preference : ");
    scanf("%d", &Mode);
    getchar();

    if (Mode == 1) {
        StartScoreMode();
    } else if (Mode == 2) {
        StartKnockoutMode();
    } else {
        printf("Negative!\n");
    }
}

// Get Score Mode
void StartScoreMode() {
    int Score = 0;
    for (int i = 0; i < QuizCount; i++) {
        printf("\nQuestion %d: %s\n", i + 1, Quiz[i].Question);
        for (int j = 0; j < Max_Options; j++) {
            printf("%d) %s\n", j + 1, Quiz[i].Options[j]);
        }

        int Answer;
        printf("Your Answer : ", Max_Options);
        scanf("%d", &Answer);
        getchar();

        if (Answer - 1 == Quiz[i].CorrectAnswerIndex) {
            printf("Correct!\n");
            Score += 5;
        } else {
            printf("Incorrect! The correct answer was %s.\n", Quiz[i].Options[Quiz[i].CorrectAnswerIndex]);
        }
    }

    printf("\nYour Total Score: %d\n", Score);
    UpdateHighScore(Score, 0); 
}

// One Life Mode
void StartKnockoutMode() {
    int Streak = 0;
    for (int i = 0; i < QuizCount; i++) {
        printf("\nQuestion %d: %s\n", i + 1, Quiz[i].Question);
        for (int j = 0; j < Max_Options; j++) {
            printf("%d) %s\n", j + 1, Quiz[i].Options[j]);
        }

        int Answer;
        printf("Your Answer : ", Max_Options);
        scanf("%d", &Answer);
        getchar();

        if (Answer - 1 == Quiz[i].CorrectAnswerIndex) {
            printf("Correct!\n");
            Streak++;
        } else {
            printf("Incorrect! The Correct Answer was %s.\n", Quiz[i].Options[Quiz[i].CorrectAnswerIndex]);
            printf("Game Over! Your Streak was: %d\n", Streak);
            UpdateHighScore(0, Streak); 
            return; 
        }
    }

    printf("You've Answered all Questions Correctly! Your longest streak : %d\n", Streak);
    UpdateHighScore(0, Streak);
}

// Track of Score and Streak
void UpdateHighScore(int Score, int Streak) {
    if (Score > HighScore) {
        HighScore = Score;
        printf("New high score: %d\n", HighScore);
    }
    if (Streak > LongestStreak) {
        LongestStreak = Streak;
        printf("New longest streak : %d\n", LongestStreak);
    }
}