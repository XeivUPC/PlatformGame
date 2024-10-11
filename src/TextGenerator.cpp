#include "TextGenerator.h"

SDL_Rect TextGenerator::GetCharRect(char c)
{
	SDL_Rect rect;
	for (int i = 0; i < charReferences.size(); i++)
	{
		if ((int)c == charReferences[i])
		{
			rect.x = i % (int)count.getX() * size.getX();
			rect.y = i / (int)count.getX() * size.getY();
			break;
		}
	}
	rect.w = size.getX();
	rect.h = size.getY();
	return rect;
}

TextGenerator::TextGenerator()
{
}

TextGenerator::~TextGenerator()
{
}

bool TextGenerator::Awake()
{
	
	return true;
}

bool TextGenerator::Start()
{
	fontTexture = Engine::GetInstance().textures->Load("Assets/Textures/Font.png");
	size = Vector2D{ 8,8 };
	int widht, height;
	Engine::GetInstance().textures->GetSize(fontTexture, widht, height);
	count = Vector2D{ (float)widht/size.getX(), (float)height/size.getY()};
	charReferences.push_back(65);
	charReferences.push_back(66);
	charReferences.push_back(67);
	charReferences.push_back(68);
	charReferences.push_back(69);
	charReferences.push_back(70);
	charReferences.push_back(71);
	charReferences.push_back(72);
	charReferences.push_back(73);
	charReferences.push_back(74);
	charReferences.push_back(75);
	charReferences.push_back(76);
	charReferences.push_back(77);
	charReferences.push_back(78);
	charReferences.push_back(79);
	charReferences.push_back(80);
	charReferences.push_back(81);
	charReferences.push_back(82);
	charReferences.push_back(83);
	charReferences.push_back(84);
	charReferences.push_back(85);
	charReferences.push_back(86);
	charReferences.push_back(87);
	charReferences.push_back(88);
	charReferences.push_back(89);
	charReferences.push_back(90);
	charReferences.push_back(97);
	charReferences.push_back(98);
	charReferences.push_back(99);
	charReferences.push_back(100);
	charReferences.push_back(101);
	charReferences.push_back(102);
	charReferences.push_back(103);
	charReferences.push_back(104);
	charReferences.push_back(105);
	charReferences.push_back(106);
	charReferences.push_back(107);
	charReferences.push_back(108);
	charReferences.push_back(109);
	charReferences.push_back(110);
	charReferences.push_back(111);
	charReferences.push_back(112);
	charReferences.push_back(113);
	charReferences.push_back(114);
	charReferences.push_back(115);
	charReferences.push_back(116);
	charReferences.push_back(117);
	charReferences.push_back(118);
	charReferences.push_back(119);
	charReferences.push_back(120);
	charReferences.push_back(121);
	charReferences.push_back(122);
	charReferences.push_back(48);
	charReferences.push_back(49);
	charReferences.push_back(50);
	charReferences.push_back(51);
	charReferences.push_back(52);
	charReferences.push_back(53);
	charReferences.push_back(54);
	charReferences.push_back(55);
	charReferences.push_back(56);
	charReferences.push_back(57);
	charReferences.push_back(46);
	charReferences.push_back(58);
	charReferences.push_back(44);
	charReferences.push_back(59);
	charReferences.push_back(40);
	charReferences.push_back(42);
	charReferences.push_back(33);
	charReferences.push_back(63);
	charReferences.push_back(32);
	charReferences.push_back(41);
	charReferences.push_back(35);
	charReferences.push_back(36);
	charReferences.push_back(37);
	charReferences.push_back(38);
	charReferences.push_back(45);
	charReferences.push_back(43);
	return true;
}

bool TextGenerator::PreUpdate()
{
	return true;
}

bool TextGenerator::Update(float dt)
{
	return true;
}

bool TextGenerator::PostUpdate()
{
	return true;
}

bool TextGenerator::CleanUp()
{
	charReferences.clear();
	return true;
}

void TextGenerator::Write(const char* text, int x, int y)
{
	int i = 0;
	while (text[i] != '\0')
	{
		SDL_Rect letter = GetCharRect(text[i]);
		Engine::GetInstance().render->DrawTexture(fontTexture, x + size.getX() * i, y, SDL_FLIP_NONE, &letter);
		i++;
	}
}
