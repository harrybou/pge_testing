// The Example!

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"

#include "olcUTIL_QuadTree.h"
#include "olcUTIL_Geometry2D.h"

class Game_JungleExplorer : public olc::PixelGameEngine 
{
public:
	Game_JungleExplorer()
	{
		sAppName = "JungleExplorer";
	}

	struct sBush
	{
		olc::vf2d vPos;
		olc::vf2d vUnitSize;
		float scale;
		size_t nImage;
	};

	olc::utils::QuadTreeContainer<sBush> treeBushes;
	std::vector<olc::Renderable> vGfxBushes;
	//olc::utils::geom2d::rect rArena = { { 0.0f, 0.0f }, { 1000.0f, 1000.0f } };
	olc::vf2d vScreenSize;
	olc::TransformedView tv;

	bool OnUserCreate() override
	{
		
		olc::utils::geom2d::rect rArena = ( olc::vf2d{ 0.0f, 0.0f }, olc::vf2d{ 2000.0f, 2000.0f } );
		vScreenSize = { float(ScreenWidth()), float(ScreenHeight()) };
		tv.Initialise(vScreenSize);
		treeBushes.resize(rArena);

		// load bushes
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree01b.png", nullptr, true);
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree02a.png", nullptr, true);
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree03a.png", nullptr, true);
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree04a.png", nullptr, true);
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree05a.png", nullptr, true);
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree06a.png", nullptr, true);
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree07a.png", nullptr, true);
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree08a.png", nullptr, true);
		vGfxBushes.emplace_back(); vGfxBushes.back().Load("images/tree09a.png", nullptr, true);

		std::cout << vGfxBushes.capacity() * sizeof(olc::Renderable) + sizeof(vGfxBushes) << std::endl;

		auto rand_float = [](const float a, const float b) {

			return float(rand()) / float(RAND_MAX) * (b - a) + a;

		};

		//populate jungle
		size_t nBushes = 100;
		for (size_t n = 0; n < nBushes; n++)
		{
			sBush bush;
			bush.nImage = rand() % vGfxBushes.size();
			std::cout << n << ", " << bush.nImage << ", " << vGfxBushes.capacity() * sizeof(olc::Renderable) + sizeof(vGfxBushes) << std::endl;
			bush.vPos = { rand_float(0.0f, rArena.size.x), rand_float(0.0f, rArena.size.y) };
			bush.scale = rand_float(50.0f, 100.0f);
			bush.vUnitSize = { 1.0f, 1.0f }; // olc::vf2d{ float(vGfxBushes[bush.nImage].Sprite()->width), float(vGfxBushes[bush.nImage].Sprite()->height) };
			//bush.vUnitSize = 1.0f / olc::vf2d{ float(vGfxBushes[bush.nImage].Sprite()->width), float(vGfxBushes[bush.nImage].Sprite()->height) };
			treeBushes.insert(bush, { bush.vPos, { bush.scale, bush.scale } });
		}

		return true;

	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		tv.HandlePanAndZoom(0);
		olc::utils::geom2d::rect rScreen = { tv.GetWorldTL(), tv.GetWorldBR() - tv.GetWorldTL() };

		for (const auto& bush : treeBushes.search(rScreen))
		{
			tv.DrawDecal(bush->item.vPos, vGfxBushes[bush->item.nImage].Decal(), bush->item.vUnitSize * bush->item.scale);
		}

		return true;
	}

	
};


/*class Example_StaticQuadTree : public olc::PixelGameEngine
{
public:
	Example_StaticQuadTree()
	{
		sAppName = "Static QuadTree";
	}

protected:
	olc::TransformedView tv;

	// An example object of something in 2D space
	struct SomeObjectWithArea
	{
		olc::vf2d vPos;
		olc::vf2d vVel;
		olc::vf2d vSize;
		olc::Pixel colour;
	};

	// A regular list of the objects
	std::list<SomeObjectWithArea> vecObjects;

	// An equivalent quad tree of the objects
	olc::utils::QuadTreeContainer<SomeObjectWithArea> treeObjects;

	float fSearchSize = 50.0f;

	// The "length" of one side of the "world" the objects reside in
	float fArea = 800.0f;

	bool bUseQuadTree = true;

public:
	bool OnUserCreate() override
	{
		// Transform View - enables Pan & Zoom
		tv.Initialise({ ScreenWidth(), ScreenHeight() });

		// Create the tree, and size it to the world
		treeObjects.resize(olc::utils::geom2d::rect(olc::vf2d{ 0.0f, 0.0f }, olc::vf2d{ fArea, fArea }));


		// Dirty random float generator
		auto rand_float = [](const float a, const float b)
		{
			return float(rand()) / float(RAND_MAX) * (b - a) + a;
		};


		// Create 1,000,000 objects, push into both containers (so 2,000,000 I suppose :P )
		for (int i = 0; i < 1500; i++)
		{
			SomeObjectWithArea ob;
			ob.vPos = { rand_float(0.0f, fArea), rand_float(0.0f, fArea) };
			//ob.vSize = { rand_float(0.1f, 100.0f), rand_float(0.1f, 100.0f) };
			ob.vSize = { 3.0f, 3.0f };
			ob.colour = olc::Pixel(rand() % 256, rand() % 256, rand() % 256);

			treeObjects.insert(ob, olc::utils::geom2d::rect(ob.vPos, ob.vSize));
			vecObjects.push_back(ob);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Tab switches between modes
		if (GetKey(olc::Key::TAB).bPressed)
			bUseQuadTree = !bUseQuadTree;

		tv.HandlePanAndZoom();

		if (GetMouseWheel() > 0 || GetKey(olc::Key::Q).bHeld)
			fSearchSize += 10.0f;
		if (GetMouseWheel() < 0 || GetKey(olc::Key::A).bHeld)
			fSearchSize -= 10.0f;

		fSearchSize = std::clamp(fSearchSize, 10.0f, 100.0f);

		// Get rectangle that equates to screen in world space
		olc::utils::geom2d::rect rScreen = { tv.GetWorldTL(), tv.GetWorldBR() - tv.GetWorldTL() };
		size_t nObjectCount = 0;

		olc::vf2d vMouse = tv.ScreenToWorld(GetMousePos());
		olc::vf2d vSearchArea = { fSearchSize, fSearchSize };
		olc::utils::geom2d::rect m(vMouse - vSearchArea / 2, vSearchArea);

		auto r = treeObjects.search(m);

		if (GetKey(olc::Key::BACK).bHeld)
		{
			for (auto& p : r)
			{
				treeObjects.remove(p);
			}
		}

		// QUAD TREE MODE
		auto tpStart = std::chrono::system_clock::now();

		// Use search function to return list of pointers to objects in that area
		for (const auto& object : treeObjects.search(rScreen))
		{
			tv.FillRectDecal(object->item.vPos, object->item.vSize, object->item.colour);
			nObjectCount++;
		}
		std::chrono::duration<float> duration = std::chrono::system_clock::now() - tpStart;


		std::string sOutput = "Quadtree " + std::to_string(nObjectCount) + "/" + std::to_string(vecObjects.size()) + " in " + std::to_string(duration.count());
		DrawStringDecal({ 4, 4 }, sOutput, olc::BLACK, { 4.0f, 8.0f });
		DrawStringDecal({ 2, 2 }, sOutput, olc::WHITE, { 4.0f, 8.0f });

		tv.FillRectDecal(m.pos, m.size, olc::Pixel(255, 255, 255, 100));

		return true;
	}
};
*/


int main()
{
	Game_JungleExplorer demo;
	if (demo.Construct(1280, 960, 1, 1, false, false))
		demo.Start();
	return 0;
}
