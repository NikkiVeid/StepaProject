#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iterator>
#include <functional>
#include "graph.hpp"

/**
 * @brief Класс графа через множество вершин.
 * @tparam WeightType Тип веса для рёбер графа.
 */
template<typename WeightType>
class AdjacencySetGraph : public Graph<WeightType> {
 private:
    std::vector<std::set<NeighborInfo<WeightType>>> adjacency_set_;
    bool directed;
 public:
    /**
     * @brief Конструктор графа с множеством вершин.
     * @param is_directed Является ли граф ориентированным (по умолчанию false).
     */
    explicit AdjacencySetGraph(bool is_directed = false) : directed(is_directed) {}
    
    /**
     * @brief Добавление ребра в граф.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @param weight Вес ребра (по умолчанию инициализируется значением по умолчанию для WeightType).
     */
    void AddEdge(int begin, int end, WeightType weight = WeightType()) override {
        if (adjacency_set_.size() <= std::max(begin, end)) {
            adjacency_set_.resize(std::max(begin, end) + 1);
        }
        adjacency_set_[begin].insert(NeighborInfo<WeightType>(end, weight));
        if (!directed) adjacency_set_[end].insert(NeighborInfo<WeightType>(begin, weight));
    }

    /**
     * @brief Удаление ребра из графа.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @param weight Вес ребра (по умолчанию инициализируется значением по умолчанию для WeightType).
     */
    void RemoveEdge(size_t begin, size_t end, WeightType weight = WeightType()) override {
        if (std::max(begin, end) >= adjacency_set_.size()) {
            return;
        }
        adjacency_set_[begin].erase({end, weight});
        if (!directed) adjacency_set_[end].erase({begin, weight});
    }
    
    /**
     * @brief Получить количество вершин в графе.
     * @return Количество вершин.
     */
    size_t Size() const { return adjacency_set_.size(); }

    /**
     * @brief Получить вес ребра между двумя вершинами.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @return Вес ребра (std::optional), если оно существует, иначе std::nullopt.
     */
    std::optional<WeightType> GetEdgeWeight(size_t begin, size_t end) const override {
        if (std::max(begin, end) >= adjacency_set_.size()) {
            return std::nullopt;
        }
        auto it = adjacency_set_[begin].find({end, WeightType{}});

        if (it != adjacency_set_[begin].end()) {
            return it->weight;
        }

        return std::nullopt;
    }

    /**
     * @brief Класс итератора по соседним вершинам.
     */
    class NeighborIterator {
     private:
        // Итератор внутри set
        typename std::set<NeighborInfo<WeightType>>::const_iterator setIter_;
        typename std::set<NeighborInfo<WeightType>>::const_iterator setEnd_;

     public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = size_t;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const size_t*;
        using reference         = size_t;

        /**
         * @brief Конструктор по умолчанию.
         */
        NeighborIterator() = default;

        /**
         * @brief Конструктор итератора.
         * @param begin Начальный индекс для итерации по множеству.
         * @param end Конечный индекс для итерации по множеству.
         */
        NeighborIterator(typename std::set<NeighborInfo<WeightType>>::const_iterator begin,
                         typename std::set<NeighborInfo<WeightType>>::const_iterator end)
            : setIter_(begin), setEnd_(end)
        {}

        /**
         * @brief Оператор разыменования.
         * @return Индекс текущей вершины-соседа.
         */
        value_type operator*() const {
            return setIter_->neighbor;
        }

        /**
         * @brief Префиксный инкремент итератора.
         * @return Ссылка на текущий объект итератора.
         */
        NeighborIterator& operator++() {
            ++setIter_;
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
            return setIter_ == other.setIter_ && setEnd_ == other.setEnd_;
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
        const std::set<NeighborInfo<WeightType>>& refSet_;
     public:
        /**
         * @brief Конструктор итератора.
         * @param refSet Ссылка на множество.
         */
        NeighborRange(const std::set<NeighborInfo<WeightType>>& refSet)
            : refSet_(refSet)
        {}

        /**
         * @brief Функция получения итератора на начало.
         * @return Итератор, который указывает на начало.
         */
        auto begin() const {
            return NeighborIterator(refSet_.begin(), refSet_.end());
        }
        /**
         * @brief Функция получения итератора конца.
         * @return Итератор, который указывает за концом.
         */
        auto end() const {
            return NeighborIterator(refSet_.end(), refSet_.end());
        }
    };

    /**
     * @brief Получить обычный итератор по соседям.
     * @param end Данная вершина.
     * @return Получение диапазона итерирования.
     */
    NeighborRange neighbors(size_t end) const {
        if (end >= adjacency_set_.size()) {
            throw std::out_of_range("You are f@cking morron! C Linus Torvalds");
        }
        return NeighborRange(adjacency_set_[end]);
    }

    /**
     * @brief Класс итератора по соседним вершинам, удовлетворяющих предикату по весу.
     * @tparam Predicate Тип предиката для фильтрации.
     */
    template <typename Predicate>
    class FilteredNeighborIterator {
        using SetIter = typename std::set<NeighborInfo<WeightType>>::const_iterator;
     public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = size_t;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const size_t*;
        using reference         = size_t;

     private:
        SetIter current_;
        SetIter end_;
        Predicate pred_;

        void skipInvalid() {
            while (current_ != end_) {
                if (pred_(current_->neighbor, current_->weight)) {
                    break;
                }
                ++current_;
            }
        }

     public:
        /**
         * @brief Конструктор по умолчанию.
         */
        FilteredNeighborIterator() = default;

        /**
         * @brief Конструктор итератора.
         * @param begin Начальный индекс для итерации по множеству.
         * @param end Конечный индекс для итерации по множеству.
         * @param pred Тип предиката для фильтрации.
         */
        FilteredNeighborIterator(SetIter begin, SetIter end, Predicate pred)
            : current_(begin), end_(end), pred_(pred) {
            skipInvalid();
        }

        /**
         * @brief Оператор разыменования.
         * @return Индекс текущей вершины-соседа.
         */
        value_type operator*() const {
            return current_->neighbor;
        }

        /**
         * @brief Префиксный инкремент итератора.
         * @return Ссылка на текущий объект итератора.
         */
        FilteredNeighborIterator& operator++() {
            ++current_;
            skipInvalid();
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
            return current_ == other.current_;
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
        const std::set<NeighborInfo<WeightType>>& refSet_;
        Predicate pred_;
     public:
        /**
         * @brief Конструктор итератора.
         * @param refSet Ссылка на множество соседей.
         * @param pred Тип предиката для фильтрации.
         */
        FilteredNeighborRange(const std::set<NeighborInfo<WeightType>>& refSet, Predicate pred)
            : refSet_(refSet), pred_(pred)
        {}

        /**
         * @brief Функция получения итератора на начало.
         * @return Итератор, который указывает на начало.
         */
        auto begin() const {
            return FilteredNeighborIterator<Predicate>(refSet_.begin(), refSet_.end(), pred_);
        }

        /**
         * @brief Функция получения итератора конца.
         * @return Итератор, который указывает за концом.
         */
        auto end() const {
            return FilteredNeighborIterator<Predicate>(refSet_.end(), refSet_.end(), pred_);
        }
    };

    /**
     * @brief Получить обычный итератор по соседям, фильтрующий соседей по условию pred(u, вес).
     * @tparam Predicate Тип предиката для фильтрации.
     * @param end Данная вершина.
     * @param pred Тип предиката для фильтрации.
     * @return Получение диапазона итерирования.
     */
    template <typename Predicate>
    auto neighborsFiltered(size_t end, Predicate pred) const {
        return FilteredNeighborRange<Predicate>(adjacency_set_[end], pred);
    }
};