// mapEditor.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <windows.h>
#include "CommDlg.h"
#include "Shlobj.h"

#include <io.h>
#include <fstream>
#include <string>

#include <queue>
#include <algorithm>
#include <vector>

using namespace std;
//D:\FILE\Project1\Maps\World1\map18.jpg
//D:\FILE\Project1\Maps\World1\data.txt
class EllipseShape : public sf::Shape
{
public:

	explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0.f, 0.f)) :
		m_radius(radius)
	{
		update();
	}

	void setRadius(const sf::Vector2f& radius)
	{
		m_radius = radius;
		update();
	}

	const sf::Vector2f& getRadius() const
	{
		return m_radius;
	}

	virtual std::size_t getPointCount() const
	{
		return 30; // fixed, but could be an attribute of the class if needed
	}

	virtual sf::Vector2f getPoint(std::size_t index) const
	{
		static const float pi = 3.141592654f;

		float angle = index * 2 * pi / getPointCount() - pi / 2;
		float x = std::cos(angle) * m_radius.x;
		float y = std::sin(angle) * m_radius.y;

		return sf::Vector2f(m_radius.x + x, m_radius.y + y);
	}

private:

	sf::Vector2f m_radius;
};

void fun_menu_new();
auto childNew = tgui::ChildWindow::create();
tgui::Gui gui;
sf::Font font;
auto childMapSrc = tgui::ChildWindow::create();

sf::Uint16 width, height;
sf::RenderWindow window;
tgui::Theme theme{ "../themes/Black.txt" }; 
std::string TCHAR2STRING(TCHAR *STR);
TCHAR * selectFile();
void setImgSrc();
void setImgDataSrc();
auto mapImgSrc = tgui::EditBox::create();
auto mapImgDataSrc = tgui::EditBox::create();
auto worldName = tgui::EditBox::create();
void createMap();
void createMapWin();
void createNewWin();
void createLeftMenu();
void createResView();
auto childRes = tgui::ChildWindow::create();

sf::Sprite sp_map;
sf::Texture texture;

sf::View viewMap, viewGui;
void changeMouseMode(sf::Int8);
sf::Cursor cursorNormal;
sf::Cursor cursorMove;
sf::Int8 mode = 0;
void getRes();

auto treeBtn = tgui::Button::create();
auto ResCanvas = tgui::Canvas::create();
void turnOffPanel(sf::Int8);
void addRes();
struct _Res
{
	std::string src;
	sf::Texture texture;
}str_res;
std::vector<_Res> ResVector;	//图片资源保存

void createAddRes();
auto childAddRes = tgui::ChildWindow::create();
auto EditResName = tgui::EditBox::create();
auto comboBoxSelectRes = tgui::ComboBox::create();
void refreshSelectRes();
auto EditTop = tgui::EditBox::create();
auto EditLeft = tgui::EditBox::create();
auto EditWidth = tgui::EditBox::create();
auto EditHeight = tgui::EditBox::create();
auto EditCenterX = tgui::EditBox::create();
auto EditCenterY = tgui::EditBox::create();
auto EditRadiusX = tgui::EditBox::create();
auto EditRadiusY = tgui::EditBox::create();

auto FileBtn = tgui::Button::create();
auto FilePanel = tgui::Panel::create();
void confirmAddRes();
auto comboBoxType = tgui::ComboBox::create();

struct str_view_res
{
	sf::Uint16 id;		//植物ID
	std::string name;
	std::string src;	//图片路径
	std::string imgName;	//图片资源名称
	sf::IntRect rect;
	sf::IntRect rectCenter;
	sf::Int8 type;
	sf::Int16 index;	//ResVector ID
}viewRes;
std::vector<str_view_res> viewResVector;	//所有资源视图	
std::vector<sf::Uint16> ResListVector;	//当前页面保存的列表

void refreshViewRes(sf::Int8 type);
auto scrollbarRes = tgui::Scrollbar::create();
void scrollbarResValueChanged();
auto caoBtn = tgui::Button::create();
void TabSelected();
auto tabsRes = tgui::Tabs::create();
void createRightMenu();
auto rightMenu = tgui::ListBox::create();
bool resEnter = false, resCanvasEnter = false, tabsResEnter = false;
bool rightMenuEnter = false;
void addRightMenu();
void ItemSelected();
sf::Int16 delId = -1;
void ResCanvasClick();

auto moveIconCanvas = tgui::Canvas::create();
sf::Int16 listIndex = -1;	//选择资源时,列表第几个
sf::Int16 resIndex = -1;	//选择资源时,资源第几个
void ResCanvasReleased();
void mouseMoved();
void createMapRes();
auto childMapRes = tgui::ChildWindow::create();
auto CanvasMapRes = tgui::Canvas::create();
auto saveChild = tgui::ChildWindow::create();
auto saveEditBox = tgui::EditBox::create();

struct str_mapRes
{
	std::string name;
	sf::Sprite sprite;
	sf::Int8 type;
	sf::Uint16 viewRes_Id;
	EllipseShape ellipse;
}mapResData;
std::vector<str_mapRes> MapResVector;
void refreshMapRes();
auto scrollbarMapRes = tgui::Scrollbar::create();
void scrollbarMapResValueChanged();
void CanvasMapResClick();
sf::VertexArray vectexArray;
sf::Texture selectIcon;
sf::Int16 selectResMapIndex = -1;
bool isImgMove = false;
bool startMove = false;
static sf::Vector2f lastAbsPos;
void setMoveIconPos(sf::Int32 index);
bool isMoveMap = false;
sf::Vector2f moveMapLastPos;
bool startMoveMap = false;
sf::Int16 mapTop = 0;
sf::Int16 mapLeft = 0;
sf::Vector2i windowSize;
bool isMapResEnter = false;

struct player_Draw_Data
{
	float posY;
	sf::Uint16 index;
}imgData;
struct playerDrawQueue
{
	bool operator ()(player_Draw_Data &a, player_Draw_Data &b)
	{
		return a.posY > b.posY;//最小值优先  
	}
};
priority_queue<player_Draw_Data, std::vector<player_Draw_Data>, playerDrawQueue>playerQueue;

void saveRes2();
void readRes2();
void saveMapRes();
void readMapRes();
void returnVal();

struct str_floder
{
	std::string src;
	std::string imgName;
}floderData;
std::vector<str_floder> floderImgSrc;
void confirmEditRes();
auto addResButton = tgui::Button::create();
auto EditResButton = tgui::Button::create();
auto RefreshResButton = tgui::Button::create();
void createChat();
auto chatbox = tgui::ChatBox::create();
std::vector<sf::Uint16> rightMenuResIdVector;
wstring stringTowstring(string str);
sf::Int8 rightMenuMode = -1;

//椭圆
/*struct str_ellipse
{
	EllipseShape ellipse;
	sf::Vector2f raiuds;
	sf::Vector2f center;
}ellipseData;
std::vector<str_ellipse>ellipseVector;
*/

auto checkbox = tgui::CheckBox::create();
std::string mapDataSrc;
auto addResCanvas = tgui::Canvas::create();
bool isaddResCanvasIn = false;
sf::Vector2f addresCanvasLastPos{0, 0};
bool isMoveAddResCanvas = false;
void resSetSprite(string str = "");
sf::Int8 addResType = -1;
void setAddResXy();
sf::RectangleShape selectRect;
sf::Int16 gridnumX, gridnumY;
void chooseDir(tgui::EditBox::Ptr editbox);
void setDefultDir();

int main()
{
	windowSize.x = 1600;
	windowSize.y = 900;
	//sf::RenderWindow window(sf::VideoMode(800, 600), "TGUI window");
	window.create(sf::VideoMode(windowSize.x, windowSize.y), "My window", { sf::Style::Titlebar | sf::Style::Close });
	gui.setTarget(window);

	moveIconCanvas->setSize(40, 40);
	gui.add(moveIconCanvas);
	font.loadFromFile("../Res/msyh.ttf");

	//viewMap.setViewport(sf::FloatRect(0.5f, 0.5f, 900.f / 1920, 1600.f / 1920));
	//viewMap.setViewport(sf::FloatRect(0.f, 0.f, 1, 1));
	viewMap.setCenter(800, 450);
	viewMap.setSize(1600, 900);

	sf::Vector2f size;
	size.x = (float)window.getSize().x;
	size.y = (float)window.getSize().y;
	viewGui.setSize(size);

	//载入鼠标图片
	sf::Image bmp;
	bmp.loadFromFile("../Res/cursor/move2.png");
	cursorMove.loadFromPixels(bmp.getPixelsPtr(), bmp.getSize(), sf::Vector2u(0, 0));
	bmp.loadFromFile("../Res/cursor/normal.png");
	cursorNormal.loadFromPixels(bmp.getPixelsPtr(), bmp.getSize(), sf::Vector2u(0, 0));
	
	//创建坐标系
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(windowSize.x / 2.f, 0)),
		sf::Vertex(sf::Vector2f(windowSize.x / 2.f, windowSize.y)),
		sf::Vertex(sf::Vector2f(0, windowSize.y / 2.f)),
		sf::Vertex(sf::Vector2f(windowSize.x, windowSize.y / 2.f))
	};
	selectRect.setSize(sf::Vector2f(32, 32));
	selectRect.setFillColor(sf::Color(255, 255, 255, 100));

	createNewWin();
	createMapWin();
	createLeftMenu();
	createResView();
	createAddRes();
	createRightMenu();
	createMapRes();
	createChat();
	readRes2();

	//选择图片时的四角
	selectIcon.loadFromFile("../Res/select.png");
	vectexArray.setPrimitiveType(sf::Quads);
	vectexArray.resize(4 * 4);
	sf::Vertex * quad;
	quad = &vectexArray[0];
	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(14, 0);
	quad[2].texCoords = sf::Vector2f(14, 14);
	quad[3].texCoords = sf::Vector2f(0, 14);

	quad = &vectexArray[4];
	quad[0].texCoords = sf::Vector2f(18, 0);
	quad[1].texCoords = sf::Vector2f(32, 0);
	quad[2].texCoords = sf::Vector2f(32, 14);
	quad[3].texCoords = sf::Vector2f(18, 14);

	quad = &vectexArray[8];
	quad[0].texCoords = sf::Vector2f(18, 18);
	quad[1].texCoords = sf::Vector2f(32, 18);
	quad[2].texCoords = sf::Vector2f(32, 32);
	quad[3].texCoords = sf::Vector2f(18, 32);

	quad = &vectexArray[12];
	quad[0].texCoords = sf::Vector2f(0, 18);
	quad[1].texCoords = sf::Vector2f(14, 18);
	quad[2].texCoords = sf::Vector2f(14, 32);
	quad[3].texCoords = sf::Vector2f(0, 32);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					sf::Vector2i fixpos = sf::Mouse::getPosition(window);
					rightMenu->setPosition((float)fixpos.x, (float)fixpos.y);
					bool isShowMenu = false;
					//资源视图上的 右键菜单
					if (resEnter == true && resCanvasEnter == true && !ResListVector.empty())
					{
						isShowMenu = true;
						rightMenuMode = 0;
						addRightMenu();	
						
						sf::Vector2f pos = ResCanvas->getAbsolutePosition();						
						delId = ResListVector.at(floor(abs(pos.y - fixpos.y) / 40.f) );
					}
					//地图上的资源 右键菜单
					else if (isMapResEnter == true)
					{
						rightMenuMode = 1;
						addRightMenu();
						
						isShowMenu = true;
						sf::Vector2f pos = CanvasMapRes->getAbsolutePosition();
						delId = floor(abs(pos.y - fixpos.y) / 30.f);
					}
					//弹出菜单 删除当前选择的资源
					else if (selectResMapIndex >= 0)
					{
						sf::Vector2f pos = MapResVector.at(selectResMapIndex).sprite.getPosition();
						sf::FloatRect floatRect = MapResVector.at(selectResMapIndex).sprite.getGlobalBounds();
						if (fixpos.x > pos.x - mapLeft && fixpos.x < pos.x - mapLeft + floatRect.width
							&& fixpos.y > pos.y - mapTop && fixpos.y < pos.y - mapTop + floatRect.height)
						{
							rightMenuMode = 3;
							addRightMenu();

							isShowMenu = true;
						}
					}
					//弹出菜单 选择当前鼠标所指的资源
					else
					{
						int i = 0;
						rightMenuResIdVector.clear();
						for (auto &data : MapResVector)
						{
							sf::Vector2f pos = data.sprite.getPosition();
							sf::FloatRect floatRect = data.sprite.getGlobalBounds();
							if (fixpos.x > pos.x - mapLeft && fixpos.x < pos.x - mapLeft + floatRect.width
								&& fixpos.y > pos.y - mapTop && fixpos.y < pos.y - mapTop + floatRect.height)
							{
								rightMenuResIdVector.push_back(i);
							}
							i++;
						}
						if (!rightMenuResIdVector.empty())
						{
							rightMenuMode = 2;
							addRightMenu();

							isShowMenu = true;
						}
						
					}

					//显示菜单
					if (isShowMenu == true)
					{
						rightMenu->setVisible(true);
						rightMenu->moveToFront();
					}
					else
					{
						//隐藏菜单
						rightMenu->setVisible(false);

						//取消地图资源的选择
						if (selectResMapIndex >= 0)
						{
							selectResMapIndex = -1;
							setMoveIconPos(-1);
						}
						//取消 移动地图
						else if (isMoveMap == true)
						{
							changeMouseMode(0);
						}
						//添加资源时, Canvas的移动
						else if (isaddResCanvasIn == true)
						{
							sf::Vector2f pos = addResCanvas->getAbsolutePosition();
							addresCanvasLastPos.x = abs(fixpos.x - pos.x);
							addresCanvasLastPos.y = abs(fixpos.y - pos.y);
							isMoveAddResCanvas = true;
						}
						//什么都没选择，则右键 移动整个地图
						else
						{
							startMoveMap = true;
							moveMapLastPos.x = fixpos.x;
							moveMapLastPos.y = fixpos.y;
						}
					}					
				}
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (tabsResEnter == false)
					{
						rightMenu->setVisible(false);
					}

					//移动地图上的资源
					sf::Vector2i fixpos = sf::Mouse::getPosition(window);
					if (selectResMapIndex >= 0)
					{
						if (isImgMove == true)
						{
							sf::Vector2f pos = MapResVector.at(selectResMapIndex).sprite.getPosition();
							sf::FloatRect floatRect = MapResVector.at(selectResMapIndex).sprite.getLocalBounds();

							startMove = true;
							lastAbsPos.x = abs(fixpos.x - pos.x);
							lastAbsPos.y = abs(fixpos.y - pos.y);
						}
						/*if (fixpos.x > pos.x - mapLeft && fixpos.x < pos.x - mapLeft + floatRect.width
							&& fixpos.y > pos.y - mapTop && fixpos.y < pos.y - mapTop + floatRect.height)
						{
							
						}
						else
						{
							selectResMapIndex = -1;
							setMoveIconPos(-1);
						}*/
					}

					//移动整个地图
					if (isMoveMap == true)
					{
						startMoveMap = true;
						moveMapLastPos.x = fixpos.x;
						moveMapLastPos.y = fixpos.y;
					}

				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					ResCanvasReleased();

					if (isImgMove == true)
					{
						startMove = false;
						isImgMove = false;
					}
					if (startMoveMap == true)
					{
						startMoveMap = false;
						sf::Vector2f center = viewMap.getCenter();
						mapLeft = center.x - windowSize.x / 2.f;
						mapTop = center.y - windowSize.y / 2.f;

					}
					
				}

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					if (isMoveAddResCanvas == true)
					{
						isMoveAddResCanvas = false;
					}
					else if (startMoveMap == true)
					{
						startMoveMap = false;
						sf::Vector2f center = viewMap.getCenter();
						mapLeft = center.x - windowSize.x / 2.f;
						mapTop = center.y - windowSize.y / 2.f;
					}
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				mouseMoved();
			}
			gui.handleEvent(event);
		}

		window.clear(sf::Color::Blue);
		window.setView(viewMap);
		window.draw(sp_map);

		int i = 0;
		for (auto &data : MapResVector)
		{
			imgData.index = i;
			imgData.posY = data.sprite.getPosition().y + data.sprite.getLocalBounds().height;
			playerQueue.push(imgData);

			i++;
		}
		while (!playerQueue.empty())
		{
			window.draw(MapResVector.at(playerQueue.top().index).sprite);
			window.draw(MapResVector.at(playerQueue.top().index).ellipse);
			playerQueue.pop();
		}
		sf::RenderStates states;
		states.texture = &selectIcon;
		window.draw(vectexArray, states);

		/*for (auto &data : ellipseVector)
		{
			window.draw(data.ellipse);
		}*/
		gui.draw();
		
		if (addResType != -1 && addResType <= 4)
		{
			sf::Vector2i fixpos = sf::Mouse::getPosition(window);
			line[0].position.x = fixpos.x + mapLeft;
			line[0].position.y = mapTop;

			line[1].position.x = fixpos.x + mapLeft;
			line[1].position.y = mapTop + windowSize.y;

			line[2].position.x = mapLeft;
			line[2].position.y = fixpos.y + mapTop;

			line[3].position.x = mapLeft + windowSize.x;
			line[3].position.y = fixpos.y + mapTop;
			if (addResType == 1)
			{
				line[0].color = sf::Color::Yellow;
				line[1].color = sf::Color::Yellow;
				line[2].color = sf::Color::Yellow;
				line[3].color = sf::Color::Yellow;
			}
			else if (addResType == 2)
			{
				line[0].color = sf::Color::Blue;
				line[1].color = sf::Color::Blue;
				line[2].color = sf::Color::Blue;
				line[3].color = sf::Color::Blue;
			}
			else if (addResType == 3)
			{
				line[0].color = sf::Color::Red;
				line[1].color = sf::Color::Red;
				line[2].color = sf::Color::Red;
				line[3].color = sf::Color::Red;
			}
			else if (addResType == 4)
			{
				line[0].color = sf::Color::Green;
				line[1].color = sf::Color::Green;
				line[2].color = sf::Color::Green;
				line[3].color = sf::Color::Green;
			}
			window.draw(line, 4, sf::Lines);
		}

		if (addResType >= 5 && isaddResCanvasIn == true)
		{
			window.draw(selectRect);
		}
		
		window.display();
	}

	return 0;

}

void createChat()
{
	width = 300;
	height = 100;	
	chatbox->setRenderer(theme.getRenderer("ChatBox"));
	chatbox->setSize(width, height);
	chatbox->setTextSize(14);
	chatbox->setPosition(0, windowSize.y - height);
	chatbox->setInheritedFont(font);
	chatbox->setLinesStartFromTop();
	chatbox->addLine("texus: Hey, this is TGUI!", sf::Color::Green);
	chatbox->addLine("Me: Looks awesome! ;)", sf::Color::Yellow);
	chatbox->addLine("texus: Thanks! :)", sf::Color::Green);
	chatbox->addLine("Me: The widgets rock ^^", sf::Color::Yellow);
	chatbox->addLine(L"阿萨德", sf::Color::Yellow);
	gui.add(chatbox);
}

//-1 - 删除
void setMoveIconPos(sf::Int32 index)
{
	sf::Vector2f pos;
	sf::Vertex * quad;
	sf::Vector2f pos2;
	sf::FloatRect floatRect;
	if (index < 0)
	{
		pos.x = -100;
		pos.y = -100;
		pos2.x = 0;
		pos2.y = 0;
		floatRect.width = 0;
		floatRect.height = 0;
		window.setMouseCursor(cursorNormal);
	}
	else
	{
		pos2 = pos = MapResVector.at(index).sprite.getPosition();
		floatRect = MapResVector.at(index).sprite.getLocalBounds();
		
	}

	quad = &vectexArray[0];
	quad[0].position = sf::Vector2f(pos.x - 7, pos.y - 7);
	quad[1].position = sf::Vector2f(pos.x + 7, pos.y - 7);
	quad[2].position = sf::Vector2f(pos.x + 7, pos.y + 7);
	quad[3].position = sf::Vector2f(pos.x - 7, pos.y + 7);

	pos.x += floatRect.width;
	quad = &vectexArray[4];
	quad[0].position = sf::Vector2f(pos.x - 7, pos.y - 7);
	quad[1].position = sf::Vector2f(pos.x + 7, pos.y - 7);
	quad[2].position = sf::Vector2f(pos.x + 7, pos.y + 7);
	quad[3].position = sf::Vector2f(pos.x - 7, pos.y + 7);

	pos = pos2;
	pos.x += floatRect.width;
	pos.y += floatRect.height;
	quad = &vectexArray[8];
	quad[0].position = sf::Vector2f(pos.x - 7, pos.y - 7);
	quad[1].position = sf::Vector2f(pos.x + 7, pos.y - 7);
	quad[2].position = sf::Vector2f(pos.x + 7, pos.y + 7);
	quad[3].position = sf::Vector2f(pos.x - 7, pos.y + 7);

	pos = pos2;
	pos.y += floatRect.height;
	quad = &vectexArray[12];
	quad[0].position = sf::Vector2f(pos.x - 7, pos.y - 7);
	quad[1].position = sf::Vector2f(pos.x + 7, pos.y - 7);
	quad[2].position = sf::Vector2f(pos.x + 7, pos.y + 7);
	quad[3].position = sf::Vector2f(pos.x - 7, pos.y + 7);
}

void showSaveChild()
{
	if (saveChild->isVisible())
	{
		saveChild->setVisible(false);
	}
	else
	{
		saveChild->setVisible(true);
	}

}

//创建地图上的资源,管理器
void createMapRes()
{
	width = 250, height = 300;
	childMapRes->setSize(width, height);
	childMapRes->connect("Closed", [&]() {
		
	});
	gui.add(childMapRes);

	CanvasMapRes->setSize(width - 20, height);
	CanvasMapRes->setPosition(0, 0);
	CanvasMapRes->connect("MousePressed", CanvasMapResClick);
	CanvasMapRes->connect("MouseEntered", [&]() {
		isMapResEnter = true;
	});
	CanvasMapRes->connect("MouseLeft", [&]() {
		isMapResEnter = false;
	});
	childMapRes->add(CanvasMapRes);

	scrollbarMapRes->setRenderer(theme.getRenderer("Scrollbar"));
	scrollbarMapRes->setPosition(width - 20, 0);
	scrollbarMapRes->setSize(18, height);
	scrollbarMapRes->setMaximum(100);
	scrollbarMapRes->setViewportSize(70);
	scrollbarMapRes->connect("ValueChanged", scrollbarMapResValueChanged);
	childMapRes->add(scrollbarMapRes);

	auto button = tgui::Button::create();
	button->setPosition(180, 0);
	button->setText(L"打开/保存");
	button->setInheritedFont(font);
	button->setSize(70, 20);
	button->connect("pressed", showSaveChild);
	childMapRes->add(button);

	saveChild->setSize(300, 100);
	saveChild->setVisible(false);
	saveChild->connect("Closed", returnVal);
	gui.add(saveChild, "saveChild");

	saveEditBox->setEnabled(false);
	saveEditBox->setRenderer(theme.getRenderer("EditBox"));
	saveEditBox->setSize(200, 25);
	saveEditBox->setTextSize(18);
	saveEditBox->setPosition(10, 10);
	saveEditBox->setDefaultText("Click to edit text...");
	saveChild->add(saveEditBox);

	button = tgui::Button::create();
	button->setPosition(220, 10);
	button->setText(L"选择...");
	button->setInheritedFont(font);
	button->setSize(50, 20);
	button->connect("pressed", chooseDir, saveEditBox);
	saveChild->add(button);
	button = tgui::Button::create();
	button->setPosition(220, 40);
	button->setText(L"默认路径");
	button->setInheritedFont(font);
	button->setSize(50, 20);
	button->connect("pressed", setDefultDir);
	saveChild->add(button);

	button = tgui::Button::create();
	button->setPosition(10, 50);
	button->setText(L"打开");
	button->setInheritedFont(font);
	button->setSize(50, 20);
	button->connect("pressed", readMapRes);
	saveChild->add(button);

	button = tgui::Button::create();
	button->setPosition(70, 50);
	button->setText(L"保存到");
	button->setInheritedFont(font);
	button->setSize(50, 20);
	button->connect("pressed", saveMapRes);
	saveChild->add(button);

	button = tgui::Button::create();
	button->setPosition(130, 50);
	button->setText(L"取消");
	button->setInheritedFont(font);
	button->setSize(50, 20);
	button->connect("pressed", [&]() {
		saveChild->setVisible(false);
	});
	saveChild->add(button);


}

void setDefultDir()
{
	saveEditBox->setText("..\\Data\\mapRes.txt");
	mapDataSrc = "..\\Data\\mapRes.txt";
}

void returnVal()
{
	saveChild->setVisible(false);
}

void chooseDir(tgui::EditBox::Ptr editbox)
{
	TCHAR * szBuffer = selectFile();
	if (szBuffer)
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen * sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, chRtn, iLen, NULL, NULL);

		std::string str(chRtn);

		std::cout<< str <<std::endl;
		mapDataSrc = str;

		editbox->setText(str);
	}
}


//地图上的资源保存到文件
void saveMapRes()
{
	if (saveEditBox->getText() == "")
	{
		if (mapDataSrc == "")
		{
			chatbox->addLine(L"数据路径不能为空");
			return;
		}
	}
	else
	{
		mapDataSrc = saveEditBox->getText();
	}

	ofstream out(mapDataSrc.c_str());
	std::string str;
	for (auto &data :MapResVector)
	{
		str += data.name + " ";
		str += std::to_string((int)data.viewRes_Id) + " ";
		str += std::to_string((int)data.sprite.getPosition().x) + " ";
		str += std::to_string((int)data.sprite.getPosition().y) + " ";			
		str += std::to_string((int)data.type) + "\n";
	}
	out << str;
	out.close();

	chatbox->addLine(L"地图上的资源成功保存到文件！");
}

//从文件读取到地图上的数据
void readMapRes()
{
	if (mapDataSrc == "")
	{
		chatbox->addLine(L"数据路径不能为空");
		return;
	}

	MapResVector.clear();
	//ellipseVector.clear();
	FILE *fp;
	//读取地图数据
	cout << "mapDataSrc:" << mapDataSrc << endl;

	fp = fopen(mapDataSrc.c_str(), "r");
	if (fp == NULL)
	{
		chatbox->addLine(L"打开文件失败");		
		return;
	}
	char charStr[255];
	int number = 0;

	if (viewResVector.empty())
	{
		std::cout << "资源视图为空！" << std::endl;
		return;
	}
	if (ResVector.empty())
	{
		std::cout << "图片资源为空！" << std::endl;
		return;
	}

	while (true)
	{
		fscanf(fp, "%s", &charStr);
		mapResData.name = charStr;

		number = -1;
		fscanf(fp, "%d", &number);
		if (number == -1)
		{
			std::cout << "跳出" << std::endl;
			break;
		}
		mapResData.viewRes_Id = number;

		sf::Uint16 viewResId = 0;
		int i = 0;
		for (auto &data : viewResVector)
		{
			if (data.id == number)
			{
				viewResId = i;
			}
			i++;
		}

		sf::Sprite sprite;
		sprite.setTexture(ResVector.at(viewResVector.at(viewResId).index).texture);
		sprite.setTextureRect(viewResVector.at(viewResId).rect);

		sf::Vector2f pos;
		fscanf(fp, "%d", &number);
		pos.x = number;

		fscanf(fp, "%d", &number);
		pos.y = number;
		sprite.setPosition(pos);

		mapResData.sprite = sprite;
		
		fscanf(fp, "%d", &number);
		mapResData.type = number;
		if (mapResData.type == 0)
		{
			sf::Vector2f center;
			center.x = viewResVector.at(viewResId).rectCenter.left;
			center.y = viewResVector.at(viewResId).rectCenter.top;
			sf::Vector2f radius;
			radius.x = viewResVector.at(viewResId).rectCenter.width;
			radius.y = viewResVector.at(viewResId).rectCenter.height;

			mapResData.ellipse.setRadius({ radius.x, radius.y });
			mapResData.ellipse.setPosition(pos.x + center.x - radius.x, pos.y + center.y - radius.y);
			mapResData.ellipse.setFillColor(sf::Color::Green);
		}
		MapResVector.push_back(mapResData);
	}

	refreshMapRes();

	fclose(fp);

	chatbox->addLine(L"读取地图上的资源成功！");
}

//地图上的资源 列表点击时
void CanvasMapResClick()
{
	if (!MapResVector.empty())
	{
		sf::Vector2i fixpos = sf::Mouse::getPosition(window);
		sf::Vector2f pos = CanvasMapRes->getAbsolutePosition();
		sf::Uint16 index = floor(abs(pos.y - fixpos.y) / 30.f);		

		selectResMapIndex = index;

		setMoveIconPos(index);

		std::cout << "mapResCanvas click" << std::endl;
	}
}

void getAllFiles(std::string path, std::vector<std::string>& files) 
{
	//文件句柄
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;  //很少用的文件信息读取结构
	std::string p;  //string类很有意思的一个赋值函数:assign()，有很多重载版本
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) 
	{
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) {  //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) 
				{
					//files.push_back(p.assign(path).append("/").append(fileinfo.name));
					files.push_back(p.assign(fileinfo.name));
					getAllFiles(p.assign(path).append("/").append(fileinfo.name), files);					
				}				
			}
			else {
				//files.push_back(p.assign(path).append("/").append(fileinfo.name));	
				files.push_back(p.assign(fileinfo.name));
			}			
		} 
		while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
			_findclose(hFile);			
	}	
}

//保存所有资源的文件
void saveRes2()
{
	//ofstream out("..\\Data\\save.txt");
	ofstream out("..\\Data\\resData.txt");
	std::string str;
	for (auto &data : viewResVector)
	{
		str += std::to_string(data.id) + " ";
		str += std::to_string(data.index) + " ";
		str += data.name + " ";
		str += std::to_string(data.rect.left) + " ";
		str += std::to_string(data.rect.top) + " ";
		str += std::to_string(data.rect.width) + " ";
		str += std::to_string(data.rect.height) + " ";

		str += std::to_string(data.rectCenter.left) + " ";
		str += std::to_string(data.rectCenter.top) + " ";
		str += std::to_string(data.rectCenter.width) + " ";
		str += std::to_string(data.rectCenter.height) + " ";

		str += data.src + " ";
		str += data.imgName + " ";
		str += std::to_string(data.type) + "\n";
		
	}
	out << str;
	chatbox->addLine(L"保存到文件成功");
	out.close();
}

//从文件读取到资源视图
void readRes2()
{
	viewResVector.clear();
	FILE *fp;

	//读取地图数据
	fp = fopen("..\\Data\\resData.txt", "r");
	if (fp == NULL) 
	{
		std::cout << "打开文件失败" << std::endl;
		return;
	}
	char charStr[255];
	int number = 0;

	while (true)
	{
		number = -1;
		fscanf(fp, "%d", &number);
		viewRes.id = number;
		if (number == -1)
		{
			std::cout << "跳出" << std::endl;
			break;
		}

		fscanf(fp, "%d", &number);
		viewRes.index = number;

		fscanf(fp, "%s", &charStr);		
		viewRes.name = charStr;

		fscanf(fp, "%d", &number);
		viewRes.rect.left = number;
		fscanf(fp, "%d", &number);
		viewRes.rect.top = number;
		fscanf(fp, "%d", &number);
		viewRes.rect.width = number;
		fscanf(fp, "%d", &number);
		viewRes.rect.height = number;

		fscanf(fp, "%d", &number);
		viewRes.rectCenter.left = number;
		fscanf(fp, "%d", &number);
		viewRes.rectCenter.top = number;
		fscanf(fp, "%d", &number);
		viewRes.rectCenter.width = number;
		fscanf(fp, "%d", &number);
		viewRes.rectCenter.height = number;

		fscanf(fp, "%s", &charStr);
		viewRes.src = charStr;

		fscanf(fp, "%s", &charStr);
		viewRes.imgName = charStr;

		fscanf(fp, "%d", &number);
		viewRes.type = number;

		bool findIt = false;
		sf::Uint16 index = 0;
		for (auto &data : ResVector)
		{
			if (data.src == viewRes.src)
			{
				findIt = true;
				break;
			}
			index++;
		}
		if (findIt == false)
		{
			str_res.src = viewRes.src;			
			str_res.texture.loadFromFile(viewRes.src);
			ResVector.push_back(str_res);
		}
		viewRes.index = index;
		viewResVector.push_back(viewRes);

	}

	fclose(fp);

	chatbox->addLine(L"读取资源成功！");

}

//资源管理器
void createResView()
{
	width = 300, height = (windowSize.y * 0.9);
	childRes->setRenderer(theme.getRenderer("ChildWindow"));
	childRes->setSize(width, height);
	childRes->setPosition(window.getSize().x - width, window.getSize().y - height);
	childRes->setTitle(L"资源管理");
	childRes->setInheritedFont(font);
	childRes->connect("Closed", [&]() {
		childRes->setVisible(false);
	});
	childRes->connect("MouseEntered", [&]() {
		resEnter = true;
	});
	childRes->connect("MouseLeft", [&]() {
		resEnter = false;
	});
	gui.add(childRes);

	//目录panel
	FilePanel->setSize(width, 25);
	FilePanel->setPosition(0, 50);
	childRes->add(FilePanel);
	//Res canvas
	ResCanvas->setSize(width - 20, 40);
	ResCanvas->setPosition(0, 50);
	ResCanvas->connect("MouseEntered", [&]() {
		resCanvasEnter = true;
	});
	ResCanvas->connect("MouseLeft", [&]() {
		resCanvasEnter = false;
	});
	ResCanvas->connect("MousePressed", ResCanvasClick);
	
	childRes->add(ResCanvas);

	auto panel = tgui::Panel::create();
	panel->setSize(width, 50);
	childRes->add(panel);

	auto button = tgui::Button::create();
	button->setPosition(0, 0);
	button->setText(L"扫描资源");
	button->setInheritedFont(font);
	button->setSize(80, 20);
	button->connect("pressed", getRes);
	panel->add(button);
	button = tgui::Button::create();
	button->setPosition(80, 0);
	button->setText(L"添加资源");
	button->setInheritedFont(font);
	button->setSize(80, 20);
	button->connect("pressed", addRes);
	panel->add(button);
	button = tgui::Button::create();
	button->setPosition(160, 0);
	button->setText(L"保存");
	button->setInheritedFont(font);
	button->setSize(50, 20);
	button->connect("pressed", saveRes2);
	panel->add(button);
	button = tgui::Button::create();
	button->setPosition(210, 0);
	button->setText(L"读取");
	button->setInheritedFont(font);
	button->setSize(50, 20);
	button->connect("pressed", readRes2);
	panel->add(button);
	
	FileBtn->setPosition(0, 20);
	FileBtn->setText(L"目录");
	FileBtn->setRenderer(theme.getRenderer("Button"));
	FileBtn->setInheritedFont(font);
	FileBtn->setSize(50, 30);
	FileBtn->connect("pressed", turnOffPanel, 0);
	panel->add(FileBtn);

	tabsRes->setRenderer(theme.getRenderer("Tabs"));
	tabsRes->setTabHeight(30);
	tabsRes->setPosition(0, 20);
	tabsRes->add(L"目录");
	tabsRes->add(L"树木");
	tabsRes->add(L"花草");
	tabsRes->setInheritedFont(font);
	tabsRes->connect("TabSelected", TabSelected);
	panel->add(tabsRes);

	scrollbarRes->setRenderer(theme.getRenderer("Scrollbar"));
	scrollbarRes->setPosition(280, 50);
	scrollbarRes->setSize(18, 300);
	scrollbarRes->setMaximum(100);
	scrollbarRes->setViewportSize(70);
	scrollbarRes->connect("ValueChanged", scrollbarResValueChanged);
	childRes->add(scrollbarRes);

}

void scrollbarMapResValueChanged()
{
	sf::Uint16 val = scrollbarMapRes->getValue();
	CanvasMapRes->setPosition(0, - val * 30);
}

//从资源视图上拖到地图上，松开鼠标时
void ResCanvasReleased()
{
	if (listIndex >= 0)
	{
		sf::Vector2i fixpos = sf::Mouse::getPosition(window);

		sf::Sprite sprite;
		sprite.setTexture(ResVector.at(viewResVector.at(ResListVector.at(listIndex)).index).texture);
		sprite.setTextureRect(viewResVector.at(ResListVector.at(listIndex)).rect);
		sprite.setPosition((float)fixpos.x + mapLeft, (float)fixpos.y + mapTop);

		mapResData.sprite = sprite;
		mapResData.name = viewResVector.at(ResListVector.at(listIndex)).name;
		mapResData.viewRes_Id = viewResVector.at(ResListVector.at(listIndex)).id;
		mapResData.type = viewResVector.at(ResListVector.at(listIndex)).type;
		//mapResData.posY = viewResVector.at(ResListVector.at(listIndex)).rectCenter.top + viewResVector.at(ResListVector.at(listIndex)).rectCenter.height;
		
		sf::Vector2f radius;
		radius.x = (float)viewResVector.at(ResListVector.at(listIndex)).rectCenter.width;
		radius.y = (float)viewResVector.at(ResListVector.at(listIndex)).rectCenter.height;		
		sf::Vector2f center;
		center.x = viewResVector.at(ResListVector.at(listIndex)).rectCenter.left;
		center.y = viewResVector.at(ResListVector.at(listIndex)).rectCenter.top;
		if (mapResData.type == 0)
		{
			mapResData.ellipse.setRadius(radius);
			mapResData.ellipse.setPosition((float)fixpos.x + mapLeft + center.x - radius.x, (float)fixpos.y + mapTop + center.y - radius.y);
			mapResData.ellipse.setFillColor(sf::Color::Green);
		}
		MapResVector.push_back(mapResData);

		moveIconCanvas->setVisible(false);

		refreshMapRes();

		listIndex = -1;

	}
}

//刷新地图上的资源
void refreshMapRes()
{
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(14);
	text.setFillColor(sf::Color::White);

	sf::Uint16 sum = 0;
	for (auto &data : MapResVector)
	{
		sum++;
	}
	CanvasMapRes->setSize(250, sum * 30);
	
	sum = 0;
	CanvasMapRes->clear();
	for (auto &data : MapResVector)
	{
		text.setPosition(0, sum * 30);
		text.setString(data.name);
		CanvasMapRes->draw(text);
		sum++;
	}

	if (sum <= 10)
	{
		scrollbarMapRes->setMaximum(sum);
		scrollbarMapRes->setViewportSize(sum);
	}
	else
	{
		scrollbarMapRes->setMaximum(sum);
		scrollbarMapRes->setViewportSize(10);
	}

	CanvasMapRes->display();

	chatbox->addLine(L"刷新地图上的资源成功！");
}

void mouseMoved()
{
	sf::Vector2i fixpos = sf::Mouse::getPosition(window);
	if(startMoveMap == true)
	{ 
		viewMap.move(moveMapLastPos.x - fixpos.x, moveMapLastPos.y - fixpos.y);
		moveMapLastPos.x = fixpos.x;
		moveMapLastPos.y = fixpos.y;
	}
	else if (isMoveAddResCanvas == true)
	{
		sf::Vector2f pos = childAddRes->getAbsolutePosition();
		addResCanvas->setPosition(fixpos.x - addresCanvasLastPos.x - pos.x - 1, fixpos.y - addresCanvasLastPos.y - pos.y - 25);
	}
	else if (addResType >= 5 && isaddResCanvasIn == true)
	{
		float size = 16;
		if (addResType == 5)
		{
			size = 16;
		}
		if (addResType == 6)
		{
			size = 32;
		}
		if (addResType == 7)
		{
			size = 64;
		}
		if (addResType == 8)
		{
			size = 128;
		}
		selectRect.setSize(sf::Vector2f(size, size));

		sf::Vector2f pos = addResCanvas->getAbsolutePosition();
		gridnumX = floor((fixpos.x - pos.x) / size);
		gridnumY = floor((fixpos.y - pos.y) / size);

		selectRect.setPosition(pos.x + gridnumX * size + mapLeft, pos.y + gridnumY * size + mapTop);
	}
	if (listIndex >= 0)
	{
		moveIconCanvas->setPosition(fixpos.x + 2, fixpos.y + 2);
	}
	else if (selectResMapIndex >= 0)
	{
		sf::Vector2f pos = MapResVector.at(selectResMapIndex).sprite.getPosition();
		sf::FloatRect floatRect = MapResVector.at(selectResMapIndex).sprite.getLocalBounds();
		
		//移动地图上的资源
		if (startMove == false)
		{
			if (fixpos.x > pos.x - mapLeft && fixpos.x < pos.x - mapLeft + floatRect.width
				&& fixpos.y > pos.y - mapTop && fixpos.y < pos.y - mapTop + floatRect.height)
			{
				window.setMouseCursor(cursorMove);
				isImgMove = true;
			}
			else
			{
				window.setMouseCursor(cursorNormal);
				isImgMove = false;
			}
		}
		else
		{
			sf::Vector2i fixpos = sf::Mouse::getPosition(window);
			sf::Int16 absX = mapLeft;
			if (absX < 0)
			{
				absX *= -1;
			}
			absX = abs(absX - lastAbsPos.x);
			sf::Int16 absY = mapTop;
			if (absY < 0)
			{
				absY *= -1;
			}
			absY = abs(absY - lastAbsPos.y);

			MapResVector.at(selectResMapIndex).sprite.setPosition(fixpos.x + mapLeft - absX, fixpos.y + mapTop - absY);

			
			if (MapResVector.at(selectResMapIndex).type == 0)
			{
				sf::Vector2f radius;
				radius.x = viewResVector.at(MapResVector.at(selectResMapIndex).viewRes_Id).rectCenter.width;
				radius.y = viewResVector.at(MapResVector.at(selectResMapIndex).viewRes_Id).rectCenter.height;
				sf::Vector2f center;
				center.x = viewResVector.at(MapResVector.at(selectResMapIndex).viewRes_Id).rectCenter.left;
				center.y = viewResVector.at(MapResVector.at(selectResMapIndex).viewRes_Id).rectCenter.top;
				MapResVector.at(selectResMapIndex).ellipse.setPosition(fixpos.x + mapLeft - absX + center.x - radius.x, fixpos.y + mapTop - absY + center.y - radius.y);
			}

			setMoveIconPos(selectResMapIndex);
		}
	}

}

//设置移动资源视图的图标
void ResCanvasClick()
{
	if (!ResListVector.empty())
	{
		sf::Vector2i fixpos = sf::Mouse::getPosition(window);
		sf::Vector2f pos = ResCanvas->getAbsolutePosition();
		listIndex = floor(abs(pos.y - fixpos.y) / 40.f);
		sf::Uint16 viewResIndex = ResListVector.at(listIndex);

		sf::Sprite sprite;
		sprite.setTexture(ResVector.at(viewResVector.at(viewResIndex).index).texture);
		sprite.setTextureRect(viewResVector.at(viewResIndex).rect);
		sprite.setScale(40.f / width, 40.f / height);

		moveIconCanvas->draw(sprite);		
		moveIconCanvas->setPosition(fixpos.x + 2, fixpos.y + 2 );
		moveIconCanvas->setVisible(true);
		moveIconCanvas->moveToFront();

	}	
}

void scrollbarResValueChanged()
{
	sf::Uint16 val = scrollbarRes->getValue();
	ResCanvas->setPosition(0, 50 - val * 40);
}

//刷新资源视图
void TabSelected()
{
	sf::Int8 index = tabsRes->getSelectedIndex();
	if (index > 0)
	{
		refreshViewRes(index - 1);
	}	
	if (index == 0)	
	{
		FilePanel->setVisible(true);
		ResCanvas->setVisible(false);
	}
	else
	{
		FilePanel->setVisible(false);
		ResCanvas->setVisible(true);
	}

}

//
void turnOffPanel(sf::Int8 id)
{

}

void addRightMenu()
{
	rightMenu->removeAllItems();

	if (rightMenuMode == -1)
	{
		return;
	}

	if (rightMenuMode == 0)
	{
		rightMenu->addItem(L"删除");
		rightMenu->addItem(L"修改");
	}
	else if (rightMenuMode == 1)
	{
		rightMenu->addItem(L"删除");
	}
	else if (rightMenuMode == 2)
	{
		for (auto &data : rightMenuResIdVector)
		{
			rightMenu->addItem(L"选择-" + stringTowstring( MapResVector.at(data).name), std::to_string(data));
		}
	}
	else if (rightMenuMode == 3)
	{
		std::cout<< "rightMenuMode == 3删除" <<std::endl;
		rightMenu->addItem(L"删除");
	}
}

//创建右键菜单
void createRightMenu()
{
	rightMenu->setRenderer(theme.getRenderer("ListBox"));
	rightMenu->setSize(200, 120);
	rightMenu->setItemHeight(24);
	rightMenu->setPosition(10, 340);
	gui.add(rightMenu);
	rightMenu->setVisible(false);
	rightMenu->setInheritedFont(font);
	rightMenu->connect("ItemSelected", ItemSelected);
	rightMenu->connect("MouseEntered", [&]() {
		tabsResEnter = true;
	});
	rightMenu->connect("MouseLeft", [&]() {
		tabsResEnter = false;
	});

}

void ItemSelected()
{
	sf::String name = rightMenu->getSelectedItem();

	if (rightMenuMode == -1)
	{
		return;
	}

	if (rightMenuMode == 2)
	{
		std::string selectId = rightMenu->getSelectedItemId();
		if (selectId == "")
		{
			return;
		}
		selectResMapIndex = std::stoi(selectId);

		setMoveIconPos(selectResMapIndex);
	}
	else if (rightMenuMode == 3 && name == L"删除" && selectResMapIndex >= 0)
	{
		MapResVector.erase(MapResVector.begin() + selectResMapIndex);
		refreshMapRes();
		setMoveIconPos(-1);

		//ellipseVector.erase(ellipseVector.begin() + selectResMapIndex);
		
		selectResMapIndex = -1;

		chatbox->addLine(L"删除完成");
	}
	else
	{
		if (name == L"删除")
		{
			//删除资源视图的
			if (rightMenuMode == 0 && delId >= 0)
			{
				viewResVector.erase(viewResVector.begin() + delId);
				TabSelected();	//刷新资源视图

				delId = -1;
			}
			//删除地图上的
			if (rightMenuMode == 1 && delId >= 0)
			{
				MapResVector.erase(MapResVector.begin() + delId);
				refreshMapRes();

				//ellipseVector.erase(ellipseVector.begin() + delId);

				delId = -1;
			}
			if (rightMenuMode == 2)
			{

			}

			chatbox->addLine(L"删除完成");
		}
		if (name == L"修改")
		{
			if (rightMenuMode == 0)
			{
				EditResName->setText(viewResVector.at(delId).name);
				comboBoxType->setSelectedItemByIndex(viewResVector.at(delId).type);

				EditLeft->setText(std::to_string(viewResVector.at(delId).rect.left));
				EditTop->setText(std::to_string(viewResVector.at(delId).rect.top));
				EditWidth->setText(std::to_string(viewResVector.at(delId).rect.width));
				EditHeight->setText(std::to_string(viewResVector.at(delId).rect.height));

				EditCenterX->setText(std::to_string(viewResVector.at(delId).rectCenter.left));
				EditCenterY->setText(std::to_string(viewResVector.at(delId).rectCenter.top));
				EditRadiusX->setText(std::to_string(viewResVector.at(delId).rectCenter.width));
				EditRadiusY->setText(std::to_string(viewResVector.at(delId).rectCenter.height));

				comboBoxSelectRes->setVisible(false);
				childAddRes->setVisible(true);
				addResButton->setVisible(false);
				EditResButton->setVisible(true);
				RefreshResButton->setVisible(false);

				string path = "../Res/res/";
				resSetSprite(path + viewResVector.at(delId).imgName);
			}
		}
	}
	
	
	rightMenu->setVisible(false);
	
}

//打开添加资源的按钮
void addRes()
{
	if (childAddRes->isVisible() == true)
	{
		childAddRes->setVisible(false);
	}
	else
	{
		childAddRes->setVisible(true);

		addResButton->setVisible(true);
		EditResButton->setVisible(false);
		RefreshResButton->setVisible(true);
		comboBoxSelectRes->setVisible(true);

	}
}

void resSetSprite(string initstr)
{
	cout << initstr << endl;
	string str;
	if (initstr == "")
	{
		str = comboBoxSelectRes->getSelectedItem();
	}
	else
	{
		str = initstr;
	}

	if (str != "")
	{
		sf::Texture texture;
		texture.loadFromFile(str);
		sf::Vector2u size = texture.getSize();
		addResCanvas->setSize(size.x, size.y);

		addResCanvas->clear();		
		sf::Sprite sprite;
		sprite.setTexture(texture);
		addResCanvas->draw(sprite);
		addResCanvas->display();
	}

}

//添加资源的窗口
void createAddRes()
{
	width = round(windowSize.x * 0.9), height = round(windowSize.y * 0.9);
	childAddRes->setSize({width, height});
	childAddRes->setRenderer(theme.getRenderer("ChildWindow"));
	childAddRes->setPosition({window.getSize().x / 2.f - width / 2.f, window.getSize().y / 2.f - height / 2.f});
	childAddRes->connect("Closed", [&]() {
		addResType = -1;
		childAddRes->setVisible(false);
	});
	gui.add(childAddRes);
	childAddRes->setVisible(false);

	auto label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setInheritedFont(font);
	label->setText(L"资源名字:");
	label->setPosition(10, 10);
	label->setTextSize(14);
	childAddRes->add(label);
	EditResName->setRenderer(theme.getRenderer("EditBox"));
	EditResName->setSize(140, 25);
	EditResName->setTextSize(14);
	EditResName->setPosition(140, 5);
	EditResName->setDefaultText("name");
	childAddRes->add(EditResName);

	auto button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(290, 5);
	button->setText("16");
	button->setSize(30, 30);
	button->setTextSize(16);
	button->connect("pressed", [&]() {
		if (addResType == 5)
		{
			addResType = -1;
		}
		else
		{
			addResType = 5;
		}
	});
	childAddRes->add(button);

	button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(330, 5);
	button->setText("32");
	button->setSize(30, 30);
	button->setTextSize(16);
	button->connect("pressed", [&]() {
		if (addResType == 6)
		{
			addResType = -1;
		}
		else
		{
			addResType = 6;
		}
	});
	childAddRes->add(button);

	button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(370, 5);
	button->setText("64");
	button->setSize(30, 30);
	button->setTextSize(16);
	button->connect("pressed", [&]() {
		if (addResType == 7)
		{
			addResType = -1;
		}
		else
		{
			addResType = 7;
		}
	});
	childAddRes->add(button);

	button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(410, 5);
	button->setText("128");
	button->setSize(30, 30);
	button->setTextSize(16);
	button->connect("pressed", [&]() {
		if (addResType == 8)
		{
			addResType = -1;
		}
		else
		{
			addResType = 8;
		}
	});
	childAddRes->add(button);

	label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setInheritedFont(font);
	label->setText(L"分类:");
	label->setPosition(10, 40);
	label->setTextSize(14);
	childAddRes->add(label);
	
	comboBoxType->setRenderer(theme.getRenderer("ComboBox"));
	comboBoxType->setSize(120, 25);
	comboBoxType->setPosition(140, 40);
	comboBoxType->addItem(L"树木");
	comboBoxType->addItem(L"草");
	comboBoxType->setSelectedItemByIndex(0);
	comboBoxType->setInheritedFont(font);
	childAddRes->add(comboBoxType);

	label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setInheritedFont(font);
	label->setText(L"资源图片:");
	label->setPosition(10, 80);
	label->setTextSize(14);
	childAddRes->add(label);

	comboBoxSelectRes->setRenderer(theme.getRenderer("ComboBox"));
	comboBoxSelectRes->setSize(200, 25);
	comboBoxSelectRes->setPosition(80, 80);
	sf::Uint16 index = 0;
	for (auto &data : floderImgSrc)
	{
		comboBoxSelectRes->addItem(data.src, std::to_string(index));
		index++;
	}
	comboBoxSelectRes->setInheritedFont(font);
	comboBoxSelectRes->connect("ItemSelected", resSetSprite);
	childAddRes->add(comboBoxSelectRes);

	RefreshResButton->setRenderer(theme.getRenderer("Button"));
	RefreshResButton->setPosition(285, 80);
	RefreshResButton->setText(L"刷新");
	RefreshResButton->setInheritedFont(font);
	RefreshResButton->setSize(50, 30);
	RefreshResButton->connect("pressed", refreshSelectRes);
	childAddRes->add(RefreshResButton);

	label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setInheritedFont(font);
	label->setText(L"xy宽高:");
	label->setPosition(10, 120);
	label->setTextSize(14);
	childAddRes->add(label);
	EditLeft->setRenderer(theme.getRenderer("EditBox"));
	EditLeft->setSize(200, 30);
	EditLeft->setTextSize(14);
	EditLeft->setPosition(80, 110);
	EditLeft->setText("0");
	childAddRes->add(EditLeft);
	EditTop->setRenderer(theme.getRenderer("EditBox"));
	EditTop->setSize(200, 30);
	EditTop->setTextSize(14);
	EditTop->setPosition(290, 110);
	EditTop->setText("0");
	childAddRes->add(EditTop);

	button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(500, 110);
	button->setText("Exit");
	button->setSize(30, 30);
	button->setText("+");
	button->setTextSize(16);
	button->connect("pressed", [&]() { 
		if (addResType == 1)
		{
			addResType = -1;
		}
		else
		{
			addResType = 1;
		}
	});
	childAddRes->add(button);

	EditWidth->setRenderer(theme.getRenderer("EditBox"));
	EditWidth->setSize(200, 30);
	EditWidth->setTextSize(14);
	EditWidth->setPosition(540, 110);
	EditWidth->setText("0");
	childAddRes->add(EditWidth);
	EditHeight->setRenderer(theme.getRenderer("EditBox"));
	EditHeight->setSize(200, 30);
	EditHeight->setTextSize(14);
	EditHeight->setPosition(740, 110);
	EditHeight->setText("0");
	childAddRes->add(EditHeight);

	button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(950, 110);
	button->setText("Exit");
	button->setSize(30, 30);
	button->setText("+");
	button->setTextSize(16);
	button->connect("pressed", [&]() {
		if (addResType == 2)
		{
			addResType = -1;
		}
		else
		{
			addResType = 2;
		}
	});
	childAddRes->add(button);

	label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setText(L"center:");
	label->setPosition(10, 150);
	label->setTextSize(14);
	childAddRes->add(label);
	EditCenterX->setRenderer(theme.getRenderer("EditBox"));
	EditCenterX->setSize(50, 30);
	EditCenterX->setTextSize(14);
	EditCenterX->setPosition(80, 140);
	EditCenterX->setText("0");
	childAddRes->add(EditCenterX);
	EditCenterY->setRenderer(theme.getRenderer("EditBox"));
	EditCenterY->setSize(50, 30);
	EditCenterY->setTextSize(14);
	EditCenterY->setPosition(130, 140);
	EditCenterY->setText("0");
	childAddRes->add(EditCenterY);

	button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(180, 140);
	button->setText("Exit");
	button->setSize(30, 30);
	button->setText("+");
	button->setTextSize(16);
	button->connect("pressed", [&]() {
		if (addResType == 3)
		{
			addResType = -1;
		}
		else
		{
			addResType = 3;
		}
	});
	childAddRes->add(button);

	label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setText(L"radius:");
	label->setPosition(220, 150);
	label->setTextSize(14);
	childAddRes->add(label);
	EditRadiusX->setRenderer(theme.getRenderer("EditBox"));
	EditRadiusX->setSize(50, 30);
	EditRadiusX->setTextSize(14);
	EditRadiusX->setPosition(270, 140);
	EditRadiusX->setText("0");
	childAddRes->add(EditRadiusX);
	EditRadiusY->setRenderer(theme.getRenderer("EditBox"));
	EditRadiusY->setSize(50, 30);
	EditRadiusY->setTextSize(14);
	EditRadiusY->setPosition(320, 140);
	EditRadiusY->setText("0");
	childAddRes->add(EditRadiusY);

	button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(380, 140);
	button->setText("Exit");
	button->setSize(30, 30);
	button->setText("+");
	button->setTextSize(16);
	button->connect("pressed", [&]() {
		if (addResType == 4)
		{
			addResType = -1;
		}
		else
		{
			addResType = 4;
		}
	});
	childAddRes->add(button);

	checkbox->setRenderer(theme.getRenderer("CheckBox"));
	checkbox->setPosition(80, 180);
	checkbox->setText(L"修改Center?");
	checkbox->setSize(25, 25);
	checkbox->setInheritedFont(font);
	//childAddRes->add(checkbox);

	addResButton->setRenderer(theme.getRenderer("Button"));
	addResButton->setPosition(80, 220);
	addResButton->setText(L"添加");
	addResButton->setInheritedFont(font);
	addResButton->setSize(60, 30);
	addResButton->connect("pressed", confirmAddRes);
	childAddRes->add(addResButton);

	EditResButton->setRenderer(theme.getRenderer("Button"));
	EditResButton->setPosition(150, 220);
	EditResButton->setText(L"修改");
	EditResButton->setInheritedFont(font);
	EditResButton->setSize(60, 30);
	EditResButton->connect("pressed", confirmEditRes);
	childAddRes->add(EditResButton);
	EditResButton->setVisible(false);

	addResCanvas->setPosition(0, 260);
	addResCanvas->setSize(round(width * 0.9), round(height * 0.9));
	addResCanvas->connect("MouseEntered", [&]() {
		isaddResCanvasIn = true;
	});
	addResCanvas->connect("MouseLeft", [&]() {
		isaddResCanvasIn = false;
	});
	addResCanvas->connect("MousePressed", setAddResXy);
	childAddRes->add(addResCanvas);
}

void setAddResXy()
{
	sf::Vector2f pos = addResCanvas->getAbsolutePosition();
	sf::Vector2i fixpos = sf::Mouse::getPosition(window);
	if (addResType == 1)
	{
		EditLeft->setText(to_string(fixpos.x - pos.x));
		EditTop->setText(to_string(fixpos.y - pos.y));
	}
	else if (addResType == 2)
	{
		string str = EditLeft->getText();
		float size = abs(atof(str.c_str()) - (fixpos.x - pos.x));
		EditWidth->setText(to_string(size));

		str = EditTop->getText();
		size = abs(atof(str.c_str()) - (fixpos.y - pos.y));
		EditHeight->setText(to_string(size));
	}
	else if (addResType == 3)
	{
		string str = EditLeft->getText();
		sf::Uint16 size = abs(atof(str.c_str()) - (fixpos.x - pos.x));
		EditCenterX->setText(to_string(size));

		str = EditTop->getText();
		size = abs(atof(str.c_str()) - (fixpos.y - pos.y));
		EditCenterY->setText(to_string(size));
	}
	else if (addResType == 4)
	{
		string str = EditCenterX->getText();
		string strx = EditLeft->getText();
		sf::Uint16 size = abs((fixpos.x - pos.x) - atof(strx.c_str()) - atof(str.c_str()));
		EditRadiusX->setText(to_string(size));
		size = size / 2.f;
		EditRadiusY->setText(to_string(size));
	}
	else if (addResType == 5)
	{
		EditLeft->setText(to_string(gridnumX * 16));
		EditTop->setText(to_string(gridnumY * 16));
		EditWidth->setText(to_string(16));
		EditHeight->setText(to_string(16));
	}
	else if (addResType == 6)
	{
		EditLeft->setText(to_string(gridnumX * 32));
		EditTop->setText(to_string(gridnumY * 32));
		EditWidth->setText(to_string(32));
		EditHeight->setText(to_string(32));
	}
	else if (addResType == 7)
	{
		EditLeft->setText(to_string(gridnumX * 64));
		EditTop->setText(to_string(gridnumY * 64));
		EditWidth->setText(to_string(64));
		EditHeight->setText(to_string(64));
	}
	else if (addResType == 8)
	{
		EditLeft->setText(to_string(gridnumX * 128));
		EditTop->setText(to_string(gridnumY * 128));
		EditWidth->setText(to_string(128));
		EditHeight->setText(to_string(128));
	}
}

//修改资源
void confirmEditRes()
{
	std::string name = EditResName->getText();
	if (name == "")
	{
		std::cout<< "名称不能为空" <<std::endl;
		return;
	}	
	sf::Int8 type = comboBoxType->getSelectedItemIndex();
	if (type < 0)
	{
		std::cout<< "类型不能为空" <<std::endl;
		return;
	}	
	std::string sleft = EditLeft->getText();
	if (sleft == "")
	{
		std::cout << "x坐标不能为空" << std::endl;
		return;		
	}	
	std::string stop = EditTop->getText();
	if (stop == "")
	{
		std::cout << "y坐标不能为空" << std::endl;
		return;
	}	
	std::string swidth = EditWidth->getText();
	if (swidth == "")
	{
		std::cout << "width不能为空" << std::endl;
		return;
	}	
	std::string sheight = EditHeight->getText();
	if (sheight == "")
	{
		std::cout << "height不能为空" << std::endl;
		return;
	}
	std::string centerX = EditCenterX->getText();
	if (centerX == "")
	{
		std::cout << "centerX不能为空" << std::endl;
		return;
	}
	std::string centerY = EditCenterY->getText();
	if (centerY == "")
	{
		std::cout << "centerY不能为空" << std::endl;
		return;
	}
	std::string centerRadiusX = EditRadiusX->getText();
	if (centerRadiusX == "")
	{
		std::cout << "centerRadiusX不能为空" << std::endl;
		return;
	}
	std::string centerRadiusY = EditRadiusY->getText();
	if (centerRadiusY == "")
	{
		std::cout << "centerRadiusY不能为空" << std::endl;
		return;
	}

	viewResVector.at(delId).name = name;
	viewResVector.at(delId).type = type;
	viewResVector.at(delId).rect.left = std::stoi(sleft);
	viewResVector.at(delId).rect.top = std::stoi(stop);
	viewResVector.at(delId).rect.width = std::stoi(swidth);
	viewResVector.at(delId).rect.height = std::stoi(sheight);
	viewResVector.at(delId).rectCenter.left = std::stoi(centerX);
	viewResVector.at(delId).rectCenter.top = std::stoi(centerY);
	viewResVector.at(delId).rectCenter.width = std::stoi(centerRadiusX);
	viewResVector.at(delId).rectCenter.height = std::stoi(centerRadiusY);

	bool isCheck = checkbox->isChecked();
	if (isCheck == true)
	{
		//viewResVector.at(delId).rectCenter.left -= viewResVector.at(delId).rect.left;
		//viewResVector.at(delId).rectCenter.top -= viewResVector.at(delId).rect.top;
	}
	//std::cout <<"sheight:" << sheight << ",rect.height:" << viewResVector.at(delId).rect.height << std::endl;

	chatbox->addLine(L"修改完成", sf::Color::Green);
	delId = -1;
	addResType = -1;
	addResButton->setVisible(true);
	EditResButton->setVisible(false);
	RefreshResButton->setVisible(true);
	comboBoxSelectRes->setVisible(true);
	childAddRes->setVisible(false);

	saveRes2();
}

//添加新的植物到资源视图
void confirmAddRes()
{
	static int imgSum = 0;
	std::string name = EditResName->getText();
	if (name == "")
	{
		std::cout<< "名称不能为空" <<std::endl;
		return;
	}
	sf::Int8 type = comboBoxType->getSelectedItemIndex();
	if (type < 0)
	{
		std::cout << "类型不能为空 " << std::endl;
		return;
	}
	std::string src = comboBoxSelectRes->getSelectedItem();

	sf::Uint16 index = comboBoxSelectRes->getSelectedItemIndex();	
	if (src == "" || index < 0)
	{
		std::cout << "资源图片不能为空 " << std::endl;
		return;
	}
	std::string ImgWidth = EditWidth->getText();
	std::string ImgHeight = EditHeight->getText();
	if (ImgWidth == "" || ImgHeight == "")
	{
		std::cout << "图片大小不能为空 " << std::endl;
		return;
	}
	std::string str = EditLeft->getText();
	sf::Uint16 x = 0;
	sf::Uint16 y = 0;
	if (str != "")
	{
		x = std::stoi((std::string)str);
	}
	str = EditTop->getText();
	if (str != "")
	{
		y = std::stoi((std::string)str);
	}
	width = std::stoi((std::string)ImgWidth);
	height = std::stoi((std::string)ImgHeight);

	std::string ImgCenterX = EditCenterX->getText();
	if (ImgCenterX == "")
	{
		std::cout << "中心点X不能为空 " << std::endl;
		return;
	}
	std::string ImgCenterY = EditCenterY->getText();
	if (ImgCenterY == "")
	{
		std::cout << "中心点Y不能为空 " << std::endl;
		return;
	}
	std::string ImgRadiusX = EditRadiusX->getText();
	if (ImgRadiusX == "")
	{
		std::cout << "阻挡快半径X不能为空 " << std::endl;
		return;
	}
	std::string ImgRadiusY = EditRadiusY->getText();
	if (ImgRadiusY == "")
	{
		std::cout << "阻挡快半径Y不能为空 " << std::endl;
		return;
	}

	viewRes.name = name;
	viewRes.rect.top = y;
	viewRes.rect.left = x;
	viewRes.rect.width = width;
	viewRes.rect.height = height;

	viewRes.rectCenter.left = std::stoi(ImgCenterX);
	viewRes.rectCenter.top = std::stoi(ImgCenterY);
	viewRes.rectCenter.width = std::stoi(ImgRadiusX);
	viewRes.rectCenter.height = std::stoi(ImgRadiusY);
	viewRes.imgName = floderImgSrc.at(index).imgName;

	index = 0;
	bool findit = false; 
	bool isSameRect = false;
	for (auto &data : ResVector)
	{
		if (data.src == src)
		{
			for (auto &data : viewResVector)
			{
				if (viewRes.rect.left == data.rect.left && viewRes.rect.top == data.rect.top && 
					viewRes.rect.width == data.rect.width && viewRes.rect.height == data.rect.height)
				{
					isSameRect = true;
					break;
				}
			}
			findit = true;
			break;
		}
		index++;
	}
	if (findit == false)
	{
		str_res.src = src;
		if (!str_res.texture.loadFromFile(src))
		{

		}
		ResVector.push_back(str_res);
	}
	if (isSameRect == true)
	{
		std::cout<< "相同图片已经添加过了" <<std::endl;
	}
	else
	{
		viewRes.src = src;
		viewRes.type = type;
		viewRes.index = index;
		if (viewResVector.empty())
		{
			viewRes.id = 0;
		}
		else
		{
			viewRes.id = viewResVector.back().id + 1;
		}
		viewResVector.push_back(viewRes);

		imgSum++;
		index = tabsRes->getSelectedIndex();

		if (index - 1 == type)
		{
			refreshViewRes(type);
		}
		chatbox->addLine(L"添加成功");
		saveRes2();
	}
	

	
	
}

//刷新资源视图
void refreshViewRes(sf::Int8 type)
{
	sf::Int16 i = 0;
	for (auto &data : viewResVector)
	{
		if (type == data.type)
		{
			i++;
		}
	}
	if (i == 0)
	{
		ResCanvas->clear();
		ResCanvas->setVisible(false);
		scrollbarRes->setVisible(false);
	}
	else if (i > 0)
	{
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(14);
		text.setFillColor(sf::Color::White);

		ResCanvas->setSize(300, i * 40);

		ResCanvas->clear();
		sf::Uint16 index = 0;
		sf::Sprite sprite;

		ResListVector.clear();

		i = 0;
		for (auto &data : viewResVector)
		{
			if (type == data.type)
			{
				sprite.setTexture(ResVector.at(data.index).texture);
				sprite.setTextureRect(data.rect);
				sprite.setScale(40.f / data.rect.width, 40.f / data.rect.height);
				sprite.setPosition(0, index * 40);
				ResCanvas->draw(sprite);	//图标
				
				text.setPosition(42, index * 40);
				text.setString(data.name);
				ResCanvas->draw(text);		//名称

				text.setString(std::to_string(index));
				text.setPosition(250, index * 40);
				ResCanvas->draw(text);		//序号

				text.setString(data.src);
				text.setPosition(42, index * 40 + 20);
				ResCanvas->draw(text);		//路径

				ResListVector.push_back(i);

				index++;
			}
			i++;
		}
		ResCanvas->display();
		std::cout << "刷新成功" << std::endl;

		index--;
		if (index <= 6)
		{
			scrollbarRes->setMaximum(index);
			scrollbarRes->setViewportSize(index);
		}
		else
		{
			scrollbarRes->setMaximum(index);
			scrollbarRes->setViewportSize(6);
		}

		scrollbarRes->setVisible(true);
	}

	
}

void refreshSelectRes()
{
	comboBoxSelectRes->removeAllItems();
	for (auto &data : floderImgSrc)
	{
		comboBoxSelectRes->addItem(data.src);
	}
	comboBoxSelectRes->setSelectedItemByIndex(0);
}

//扫描文件夹内的图片
void getRes()
{
	floderImgSrc.clear();

	std::string path;
	path = "../Res/res";
	//path = "../Res/res";
	std::vector<std::string> str;
	getAllFiles(path, str);
	sf::Uint32 sum = 0;
	sf::Vector2i size{ (int)childRes->getSize().x, 40 };

	for (auto &data :str)
	{
		auto label = tgui::Label::create();
		label->setText(path + "/" + data);
		label->setPosition(45, sum * size.y);
		label->setTextSize(14);
		label->setSize(size.x, size.y);
		FilePanel->add(label);

		floderData.src = path + "/" + data;
		floderData.imgName = data;
		floderImgSrc.push_back(floderData);
		
		std::cout<< "name:" << data <<std::endl;

		sum++;
	}
	FilePanel->setSize(size.x, size.y * sum);
}

//创建左侧的菜单按钮
void createLeftMenu()
{
	//顶部菜单
	auto button = tgui::Button::create();
	button->setPosition(0, 0);
	button->setText("Load");
	button->setSize(40, 20);
	button->connect("pressed", fun_menu_new);
	gui.add(button);

	button = tgui::Button::create();
	button->setSize(40, 20);
	button->setText(L"拖动");
	button->setInheritedFont(font);
	button->setPosition(0, 20);
	button->connect("pressed", changeMouseMode, 1);
	gui.add(button);

	button = tgui::Button::create();
	button->setSize(40, 20);
	button->setText(L"复位");
	button->setInheritedFont(font);
	button->setPosition(0, 40);
	button->connect("pressed", changeMouseMode, 2);
	gui.add(button);
}

//改变鼠标模式
void changeMouseMode(sf::Int8 mode)
{
	if (mode == 0)
	{
		window.setMouseCursor(cursorNormal);
		isMoveMap = false;
		mode = 0;
	}
	else if (mode == 1)
	{
		window.setMouseCursor(cursorMove);
		mode = 1;
		isMoveMap = true;
	}
	else if (mode == 2)
	{
		viewMap.setCenter(sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f));
		mapLeft = 0;
		mapTop = 0;
	}
}

//新建地图的窗口
void createNewWin()
{
	width = 350, height = 200;
	childNew->setRenderer(theme.getRenderer("ChildWindow"));
	childNew->setSize(width, height);
	childNew->setPosition(window.getSize().x / 2.f - width / 2.f, window.getSize().y / 2.f - height / 2.f);
	childNew->setTitle("Child window");
	childNew->connect("Closed", [&]() {
		childNew->setVisible(false);
	});
	childNew->setVisible(false);

	gui.add(childNew);
	auto label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setInheritedFont(font);
	label->setText(L"地图名字(World):");
	label->setPosition(10, 10);
	label->setTextSize(14);
	childNew->add(label);
	worldName->setRenderer(theme.getRenderer("EditBox"));
	worldName->setSize(140, 25);
	worldName->setTextSize(14);
	worldName->setPosition(140, 5);
	worldName->setText("map1");
	childNew->add(worldName);
	label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setInheritedFont(font);
	label->setText(L"地图图片:");
	label->setPosition(10, 40);
	label->setTextSize(14);
	childNew->add(label);
	mapImgSrc->setRenderer(theme.getRenderer("EditBox"));
	mapImgSrc->setSize(180, 25);
	mapImgSrc->setTextSize(14);
	mapImgSrc->setPosition(80, 35);
	mapImgSrc->setText("..\\maps\\map24.gif");
	childNew->add(mapImgSrc);
	auto button = tgui::Button::create();
	button->setPosition(265, 35);
	button->setInheritedFont(font);
	button->setText(L"···");
	button->setSize(25, 25);
	button->connect("pressed", setImgSrc);
	childNew->add(button);

	label = tgui::Label::create();
	label->setRenderer(theme.getRenderer("Label"));
	label->setInheritedFont(font);
	label->setText(L"选择地图数据:");
	label->setPosition(10, 70);
	label->setTextSize(14);
	childNew->add(label);

	mapImgDataSrc->setRenderer(theme.getRenderer("EditBox"));
	mapImgDataSrc->setSize(180, 25);
	mapImgDataSrc->setTextSize(14);
	mapImgDataSrc->setPosition(120, 65);
	mapImgDataSrc->setText("..\\Data\\mapRes.txt");
	childNew->add(mapImgDataSrc);

	button = tgui::Button::create();
	button->setPosition(305, 65);
	button->setInheritedFont(font);
	button->setText(L"···");
	button->setSize(25, 25);
	button->connect("pressed", setImgDataSrc);
	childNew->add(button);

	button = tgui::Button::create();
	button->setRenderer(theme.getRenderer("Button"));
	button->setPosition(75, 100);
	button->setInheritedFont(font);
	button->setText(L"创建");
	button->setSize(50, 30);
	button->connect("pressed", createMap);
	childNew->add(button);
}

//点击创建按钮后，开始创建
void createMap()
{
	std::string name;
	name = worldName->getText();

	std::string src;
	src = mapImgSrc->getText();
	
	if (name != "" && src != "")
	{
		if (!texture.loadFromFile(src))
		{
			std::cout<< "加载图片失败" <<std::endl;
		}
		else
		{
			MapResVector.clear();
			//ellipseVector.clear();
			std::cout << "加载图片成功" << std::endl;

			auto label = tgui::Label::create();
			label->setRenderer(theme.getRenderer("Label"));
			label->setText(src);
			label->setPosition(0, 0);
			label->setTextSize(14);
			label->setSize(childMapSrc->getSize().x, 50);
			childMapSrc->add(label);

			childNew->setVisible(false);

			sp_map.setTexture(texture);
			sp_map.setPosition(0, 0);
			sp_map.setTextureRect(sf::IntRect(0, 0, 1920, 1920));

			std::cout << "创建成功" << std::endl;
		}
	}
	else if (name == "")
	{
		std::cout << "地图名字不能为空" << std::endl;
	}
	else if (src == "")
	{
		std::cout<< "地图图片不能为空" <<std::endl;
	}

	mapDataSrc = mapImgDataSrc->getText();
	if (mapDataSrc == "")
	{
		std::cout << "数据地址不能为空" << std::endl;
	}
	else
	{
		readMapRes();
	}

	chatbox->addLine(mapDataSrc);
}

void setImgSrc()
{
	TCHAR * szBuffer = selectFile();
	if (szBuffer)
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen * sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, chRtn, iLen, NULL, NULL);
		std::string str(chRtn);

		mapImgSrc->setText(str);
	}
}
void setImgDataSrc()
{
	TCHAR * szBuffer = selectFile();
	if (szBuffer)
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen * sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, chRtn, iLen, NULL, NULL);
		std::string str(chRtn);

		mapImgDataSrc->setText(str);
	}
}
TCHAR * selectFile()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = L"从下面选择文件或文件夹:";
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return 0;
	}
	SHGetPathFromIDList(idl, szBuffer);
	return szBuffer;
}

void fun_menu_new()
{
	if (childNew->isVisible() == true)
	{
		childNew->setVisible(false);
	}
	else
	{
		childNew->setVisible(true);
	}
}

//创建地图迷你预览窗口
void createMapWin()
{
	width = 300, height = 160;
	childMapSrc->setRenderer(theme.getRenderer("ChildWindow"));
	childMapSrc->setSize(width, height);
	childMapSrc->setPosition(window.getSize().x - width, 0);
	childMapSrc->setTitle("Map src");
	childMapSrc->connect("Closed", [&]() {
		childMapSrc->setVisible(false);
	});
	gui.add(childMapSrc);
}

std::string TCHAR2STRING(TCHAR *STR)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen * sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	delete chRtn;
	return str;
}

wstring stringTowstring(string str) {
	wstring result;      //获取缓冲区大小，并申请空间，缓冲区大小按字符计算      
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];      //多字节编码转换成宽字节编码      
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾      //删除缓冲区并返回值      
	result.append(buffer);
	delete[] buffer;
	return result;
}



