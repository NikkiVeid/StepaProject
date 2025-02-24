#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <functional>
#include "graph.hpp"

/**
 * @brief Класс графа через список ребер.
 * @tparam WeightType Тип веса для рёбер графа.
 */
template<typename WeightType>
class EdgeListGraph : public Graph<WeightType> {
 private:
    std::unordered_set<size_t> vertexes;
    std::vector<Edge<WeightType>> edges_;
    bool directed;
 public:
    /**
     * @brief Конструктор графа из списка ребер.
     * @param is_directed Является ли граф ориентированным (по умолчанию false).
     */
    explicit EdgeListGraph(bool is_directed = false) : directed(is_directed) {}
    
    /**
     * @brief Добавление ребра в граф.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @param weight Вес ребра (по умолчанию инициализируется значением по умолчанию для WeightType).
     */
    void AddEdge(int begin, int end, WeightType weight = WeightType()) override {
        edges_.emplace_back(begin, end, weight);
        vertexes.insert(begin);
        vertexes.insert(end);
        if (!directed) edges_.emplace_back(end, begin, weight);
    }
    
    /**
     * @brief Удаление ребра из графа.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     */
    void RemoveEdge(size_t begin, size_t end, WeightType weight = WeightType()) override {
        auto it = edges_.begin();
        for (auto end = edges_.end(); it != end; ++it) {
            if (it->from == begin && it->to == end) {
                break;
            }
        }
        if (it == edges_.end()) {
            return;
        }
        edges_.erase(it);
        if (!directed) edges_.erase(it);
    }

    /*/**
     * @brief Получить количество вершин в графе.
     * @return Количество вершин.
     */
    size_t Size() const {
      std::unordered_set<size_t> vertexes;
      for (const Edge<WeightType>& el : edges_) {
        vertexes.insert(el.from);
        vertexes.insert(el.to);
      }
      return vertexes.size();
    }

    /**
     * @brief Получить вес ребра между двумя вершинами.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @return Вес ребра (std::optional), если оно существует, иначе std::nullopt.
     */
    std::optional<WeightType> GetEdgeWeight(size_t begin, size_t end) const override{
        for (auto &e : edges_) {
            if (e.from == begin && e.to == end) {
                return e.weight;
            }
        }
        return std::nullopt;
    }
    
    /**
     * @brief Класс итератора по соседним вершинам.
     */
    class NeighborIterator {
     private:
        const std::vector<Edge<WeightType>>* edgesPtr_{nullptr};
        size_t currentIndex_{0};
        size_t source_{0};

        void advanceToNextEdge() {
            while (currentIndex_ < edgesPtr_->size()) {
                if ((*edgesPtr_)[currentIndex_].from == source_) {
                    break;
                }
                ++currentIndex_;
            }
        }

     public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = size_t;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const size_t*;
        using reference         = const size_t&;

        /**
         * @brief Конструктор по умолчанию.
         */
        NeighborIterator() = default;

        /**
         * @brief Конструктор итератора.
         * @param edgesPtr Указатель на список ребер.
         * @param startIndex Начальный индекс для итерации.
         * @param source Исходная вершина.
         */
        NeighborIterator(const std::vector<Edge<WeightType>>* edgesPtr, size_t startIndex, size_t source)
            : edgesPtr_(edgesPtr), currentIndex_(startIndex), source_(source) {
            advanceToNextEdge();
        }

        /**
         * @brief Оператор разыменования.
         * @return Индекс текущей вершины-соседа.
         */
        value_type operator*() const {
            return (*edgesPtr_)[currentIndex_].to;
        }

        /**
         * @brief Префиксный инкремент итератора.
         * @return Ссылка на текущий объект итератора.
         */
        NeighborIterator& operator++() {
            ++currentIndex_;
            advanceToNextEdge();
            return *this;
        }

        /**
         * @brief Постфиксный инкремент итератора.
         * @return Копия текущего объекта итератора до инкремента.
         */
        NeighborIterator operator++(int) {
            NeighborIterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * @brief Оператор равенства.
         * @param other Другой итератор для сравнения.
         * @return true, если итераторы равны, иначе false.
         */
        bool operator==(const NeighborIterator& other) const {
            return currentIndex_ == other.currentIndex_ && edgesPtr_ == other.edgesPtr_;
        }

        /**
         * @brief Оператор неравенства.
         * @param other Другой итератор для сравнения.
         * @return true, если итераторы не равны, иначе false.
         */
        bool operator!=(const NeighborIterator& other) const {
            return !(*this == other);
        }
    };

    /**
     * @brief Вспомогательный класс-диапазон для удобства итерирования.
     */
    class NeighborRange {
     private:
        const std::vector<Edge<WeightType>>* edgesPtr_{nullptr};
        size_t source_{0};
     public:
        /**
         * @brief Конструктор итератора.
         * @param edgesPtr Указатель на список ребер.
         * @param source Исходная вершина.
         */
        NeighborRange(const std::vector<Edge<WeightType>>* edgesPtr, size_t source)
            : edgesPtr_(edgesPtr), source_(source)
        {}

        /**
         * @brief Функция получения итератора на начало.
         * @return Итератор, который указывает на начало.
         */
        auto begin() const {
            return NeighborIterator(edgesPtr_, 0, source_);
        }

        /**
         * @brief Функция получения итератора конца.
         * @return Итератор, который указывает за концом.
         */
        auto end() const {
            return NeighborIterator(edgesPtr_, edgesPtr_ ? edgesPtr_->size() : 0, source_);
        }
    };

    /**
     * @brief Получить обычный итератор по соседям.
     * @param end Данная вершина.
     * @return Получение диапазона итерирования.
     */
    NeighborRange neighbors(size_t end) const {
        return NeighborRange(&edges_, end);
    }

    /**
     * @brief Класс итератора по соседним вершинам, удовлетворяющих предикату по весу.
     * @tparam Predicate Тип предиката для фильтрации.
     */
    template <typename Predicate>
    class FilteredNeighborIterator {
        using EdgesVector = std::vector<Edge<WeightType>>;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = size_t;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const size_t*;
        using reference         = const size_t&;

    private:
        const EdgesVector* edgesPtr_{nullptr};
        size_t currentIndex_{0};
        size_t source_{0};
        Predicate pred_;

        void advanceToNextEdge() {
            while (currentIndex_ < edgesPtr_->size()) {
                const auto &e = (*edgesPtr_)[currentIndex_];
                if (e.from == source_ && pred_(e.to, e.weight)) {
                    break;
                }
                ++currentIndex_;
            }
        }

    public:
        /**
         * @brief Конструктор по умолчанию.
         */
        FilteredNeighborIterator() = default;

        /**
         * @brief Конструктор итератора.
         * @param edgesPtr Указатель на строку матрицы смежности.
         * @param startIndex Начальный индекс для итерации.
         * @param source Исходная вершина.
         * @param pred Тип предиката для фильтрации.
         */
        FilteredNeighborIterator(const EdgesVector* edgesPtr, size_t startIndex,
                                 size_t source, Predicate pred)
            : edgesPtr_(edgesPtr),
              currentIndex_(startIndex),
              source_(source),
              pred_(pred)
        {
            advanceToNextEdge();
        }

        /**
         * @brief Оператор разыменования.
         * @return Индекс текущей вершины-соседа.
         */
        value_type operator*() const {
            return (*edgesPtr_)[currentIndex_].to;
        }

        /**
         * @brief Префиксный инкремент итератора.
         * @return Ссылка на текущий объект итератора.
         */
        FilteredNeighborIterator& operator++() {
            ++currentIndex_;
            advanceToNextEdge();
            return *this;
        }

        /**
         * @brief Постфиксный инкремент итератора.
         * @return Копия текущего объекта итератора до инкремента.
         */
        FilteredNeighborIterator operator++(int) {
            FilteredNeighborIterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * @brief Оператор равенства.
         * @param other Другой итератор для сравнения.
         * @return true, если итераторы равны, иначе false.
         */
        bool operator==(const FilteredNeighborIterator& other) const {
            return currentIndex_ == other.currentIndex_ && edgesPtr_ == other.edgesPtr_;
        }

        /**
         * @brief Оператор неравенства.
         * @param other Другой итератор для сравнения.
         * @return true, если итераторы не равны, иначе false.
         */
        bool operator!=(const FilteredNeighborIterator& other) const {
            return !(*this == other);
        }
    };

    /**
     * @brief Класс фильтрованного итератора по соседним вершинам, удовлетворяющих предикату по весу.
     * @tparam Predicate Тип предиката для фильтрации.
     */
    template <typename Predicate>
    class FilteredNeighborRange {
        const std::vector<Edge<WeightType>>* edgesPtr_{nullptr};
        size_t source_{0};
        Predicate pred_;
    public:
        /**
         * @brief Конструктор итератора.
         * @param edgesPtr Указатель на список ребер.
         * @param source Исходная вершина.
         * @param Predicate Тип предиката для фильтрации.
         */
        FilteredNeighborRange(const std::vector<Edge<WeightType>>* edgesPtr, size_t source, Predicate pred)
            : edgesPtr_(edgesPtr), source_(source), pred_(pred)
        {}

        /**
         * @brief Функция получения итератора на начало.
         * @return Итератор, который указывает на начало.
         */
        auto begin() const {
            return FilteredNeighborIterator<Predicate>(edgesPtr_, 0, source_, pred_);
        }

        /**
         * @brief Функция получения итератора конца.
         * @return Итератор, который указывает за концом.
         */
        auto end() const {
            return FilteredNeighborIterator<Predicate>(
                edgesPtr_,
                edgesPtr_ ? edgesPtr_->size() : 0,
                source_,
                pred_
            );
        }
    };

    /**
     * @brief Получить обычный итератор по соседям, фильтрующий соседей по условию pred(begin, вес).
     * @tparam Predicate Тип предиката для фильтрации.
     * @param end Данная вершина.
     * @param pred Тип предиката для фильтрации.
     * @return Получение диапазона итерирования.
     */
    template <typename Predicate>
    FilteredNeighborRange<Predicate> neighborsFiltered(size_t end, Predicate pred) const {
        return FilteredNeighborRange<Predicate>(&edges_, end, pred);
    }
};