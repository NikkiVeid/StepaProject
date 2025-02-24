#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <functional>

/**
 * @brief Виртуальный класс графа.
 * @tparam WeightType Тип веса для рёбер графа.
 */
template<typename WeightType>
class Graph {
 public:
    /**
     * @brief Добавление ребра в граф.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @param weight Вес ребра (по умолчанию инициализируется значением по умолчанию для WeightType).
     */
    virtual void AddEdge(int begin, int end, WeightType weight = WeightType()) = 0;

    /**
     * @brief Удаление ребра из графа.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @param weight Вес ребра (по умолчанию инициализируется значением по умолчанию для WeightType).
     */
    virtual void RemoveEdge(size_t begin, size_t end, WeightType weight = WeightType()) = 0;

    /**
     * @brief Получить вес ребра между двумя вершинами.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @return Вес ребра (std::optional), если оно существует, иначе std::nullopt.
     */
    virtual std::optional<WeightType> GetEdgeWeight(size_t begin, size_t end) const = 0;

    /**
     * @brief Дефолтный деструктор.
     */
    virtual ~Graph() = default;
};

/**
 * @brief Структура для хранения ребра.
 * @tparam WeightType Тип веса для рёбер графа.
 */
template<typename WeightType>
struct Edge {
    size_t from;       //!< Начальная вершина
    size_t to;         //!< Конечная вершина
    WeightType weight; //!< Вес ребра
};

/**
 * @brief Структура для хранения информации о соседе.
 * @tparam WeightType Тип веса для рёбер графа.
 */
template<typename WeightType>
struct NeighborInfo {
    size_t neighbor;   //!< Номер соседа
    WeightType weight; //!< Вес ребра

    /**
     * @brief Конструктор для структуры NeighborInfo.
     * @param neigh Номер соседа.
     * @param weight Вес ребра.
     */
    NeighborInfo(size_t neigh, WeightType weight) :  neighbor(neigh), weight(weight) {}

    // Чтобы std::set работал, определим оператор <
    bool operator<(const NeighborInfo& other) const {
        return neighbor < other.neighbor;
    }
};

/**
 * @brief Простая enum-ка для статусов вершины при обходе.
 */
enum class Color { White, Gray, Black };

/**
 * @brief Базовый класс Visitor для обхода графа.
 * 
 * Этот класс предоставляет шаблонные методы, которые могут быть переопределены 
 * в пользовательских посетителях для обработки различных событий во время обхода графа.
 */
struct Visitor {
    /**
     * @brief Вызывается при обнаружении вершины.
     * @tparam Graph Тип графа.
     * @param begin Индекс вершины.
     * @param g Граф.
     */
    template <typename Graph>
    void discoverVertex(size_t /*begin*/, const Graph& /*g*/) {}

    /**
     * @brief Вызывается при начале обработки вершины.
     */
    template <typename Graph>
    void examineVertex(size_t /*begin*/, const Graph& /*g*/) {}

    /**
     * @brief Вызывается при обработке ребра.
     */
    template <typename Graph>
    void examineEdge(size_t /*begin*/, size_t /*end*/, const Graph& /*g*/) {}

    /**
     * @brief Вызывается для ребра, которое является частью дерева обхода.
     */
    template <typename Graph>
    void treeEdge(size_t /*begin*/, size_t /*end*/, const Graph& /*g*/) {}

    /**
     * @brief Вызывается для обратного или поперечного ребра.
     */
    template <typename Graph>
    void nonTreeEdge(size_t /*begin*/, size_t /*end*/, const Graph& /*g*/) {}

    /**
     * @brief Вызывается при завершении обработки вершины.
     */
    template <typename Graph>
    void finishVertex(size_t /*begin*/, const Graph& /*g*/) {}
};
