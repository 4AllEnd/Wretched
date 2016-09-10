/*
#include <GL/glew.h>
#include <Wretched/Dialogue.h>
struct Character{
	std::string name = "Teddy";
	std::string race = "Human";
};
void main(){

	system("TITLE Schism SubEngine:Text");
	Character Teddy;
	const char *text = "Reginald \"Reggie\" Wilson :\n$$$$$$ makes the world go around, doesn't it kiddo? So, $player.name$...\nHow have things been $Special{0}$?\n$Special{1}$\nWould you like to buy something, prices starting at only US $$$$100,000.00?\n";
	Wretched::Dialogue::Dialogue dia=Wretched::Dialogue::tokenize(text);
	dia.substitutetoken("player.name", Teddy.name.c_str());
	for (auto &each : dia) {
		std::printf("%s", each.c_str());
	}
	std::cin.get();
}*/
