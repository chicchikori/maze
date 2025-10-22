#pragma once

#include <memory>
#include <thread>
#include <unordered_set>
#include <vector>
#include "../../Window/PopUp.h"
#include "../ControlMaze/Control.h"
#include "../Util/uf.h"
#include "Road.h"


namespace Uf
{
    class UnionFind
    {
    public:
        UnionFind() = default;
        explicit UnionFind(const std::shared_ptr<Settings>& setting, const std::shared_ptr<Window>& window);
        void Exit();
        void Recenter(Cont::Control& controller) const;
        void Create(Cont::Control& controller);

        static std::atomic<bool> running;
        static std::atomic<bool> isRunThread;
        static std::atomic<int> progress;

        std::shared_ptr<rc::Road> GetRoad() const
        {
            return road;
        }
        void ProcessBarRender();

    private:
        void Init();
        bool Find(Cont::Control& controller);
        bool Unite(const std::shared_ptr<Point>& t1, const std::shared_ptr<Point>& t2);
        static std::shared_ptr<Point> Root(const std::shared_ptr<Point>& point);

        std::shared_ptr<Settings> settings;
        std::shared_ptr<Window> window;
        std::shared_ptr<rc::Road> road;
        std::vector<std::shared_ptr<Point>> points; // w * h
        std::vector<std::shared_ptr<Edge>> edges;
        std::unordered_set<std::shared_ptr<Edge>> trees;

        PopUp pp;
        std::jthread th1;
        bool isAnswer = true;
        bool isGoal = false;
    };
} // namespace Uf
