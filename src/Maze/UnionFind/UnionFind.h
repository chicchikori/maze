#pragma once

#include <future>
#include <memory>
#include <thread>
#include <unordered_set>
#include <vector>
#include "../../Window/PopUp.h"

#include "../Util/uf.h"


namespace Uf
{
    class UnionFind
    {
    public:
        UnionFind() = default;
        explicit UnionFind( const std::shared_ptr<Window>& window);
        void Exit();
        void Create(std::promise<std::unordered_set<std::shared_ptr<Uf::Tree>>>& promiseTree,
                    const MazePeference& pref);
        std::unordered_set<std::shared_ptr<Uf::Tree>> Create(const MazePeference& pref);

        static std::atomic<bool> running;
        static std::atomic<bool> isRunThread;
        static std::atomic<int> progress;

        /* std::shared_ptr<rc::Road> GetRoad() const
        {
            return road;
        } */
        void ProcessBarRender();

    protected:
        std::jthread th1; // NOLINT

    private:
        std::unordered_set<std::shared_ptr<Uf::Tree>> Find();
        static bool Unite(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2);
        static std::shared_ptr<Node> Root(const std::shared_ptr<Node>& node);

        std::shared_ptr<Settings> settings;
        void Init(const MazePeference& pref);
        std::shared_ptr<Window> window;
        std::vector<std::shared_ptr<Node>> nodes; // w * h
        std::vector<std::shared_ptr<Tree>> trees; // (w-1)*h + w*(h-1)

        PopUp pp;
        bool isAnswer = true;
        bool isGoal = false;
    };
} // namespace Uf
