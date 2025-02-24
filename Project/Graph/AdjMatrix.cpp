#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <functional>
#include "graph.hpp"

/**
 * @brief Класс графа через матрицу смежности.
 * @tparam WeightType Тип веса для рёбер графа.
 */
template<typename WeightType>
class AdjacencyMatrixGraph : public Graph<WeightType> {
    std::vector<std::vector<std::optional<WeightType>>> matrix_;
    bool directed;
 public:
    /**
     * @brief Конструктор графа с матрицей смежности.
     * @param vertices Количество вершин в графе.
     * @param is_directed Является ли граф ориентированным (по умолчанию false).
     */
    explicit AdjacencyMatrixGraph(int vertices, bool is_directed = false) :
        matrix_(vertices, std::vector<std::optional<WeightType>>(vertices, std::nullopt)),
        directed(is_directed) {}
    
    /**
     * @brief Добавление ребра в граф.
     * @param u Начальная вершина.
     * @param end Конечная вершина.
     * @param weight Вес ребра (по умолчанию инициализируется значением по умолчанию для WeightType).
     */
    void AddEdge(int begin, int end, WeightType weight = WeightType()) override {
        matrix_[begin][end] = weight;
        if (!directed) {
            matrix_[end][begin] = weight;
        }
    }

    /**
     * @brief Удаление ребра из графа.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @param weight Вес ребра (по умолчанию инициализируется значением по умолчанию для WeightType).
     */
    void RemoveEdge(size_t begin, size_t end, WeightType weight = WeightType()) override {
        matrix_[begin][end].reset();
        if (!directed) {
            matrix_[end][begin].reset();
        }
    }

    /**
     * @brief Получить количество вершин в графе.
     * @return Количество вершин.
     */
    size_t Size() const { return matrix_.size(); }

    /**
     * @brief Получить вес ребра между двумя вершинами.
     * @param begin Начальная вершина.
     * @param end Конечная вершина.
     * @return Вес ребра (std::optional), если оно существует, иначе std::nullopt.
     */
    std::optional<WeightType> GetEdgeWeight(size_t begin, size_t end) const override {
        return matrix_[begin][end];
    }

    /**
     * @brief Класс итератора по соседним вершинам.
     */
    class NeighborIterator {
     public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = size_t;     // Индекс соседней вершины
        using difference_type   = std::ptrdiff_t;
        using pointer           = const size_t*;
        using reference         = const size_t&;

     private:
        const std::vector<std::optional<WeightType>>* rowPtr_{nullptr};
        size_t current_{0};
        size_t rowSize_{0};

        void advanceToNextEdge() {
            while (current_ < rowSize_ && !(*rowPtr_)[current_].has_value()) {
                ++current_;
            }
        }

    public:
        /**
         * @brief Конструктор по умолчанию.
         */
        NeighborIterator() = default;

        /**
         * @brief Конструктор итератора.
         * @param rowPtr Указатель на строку матрицы смежности.
         * @param startIndex Начальный индекс для итерации.
         */
        NeighborIterator(const std::vector<std::optional<WeightType>>* rowPtr, size_t startIndex)
            : rowPtr_(rowPtr), current_(startIndex) {
            if (rowPtr_) {
                rowSize_ = rowPtr_->size();
                advanceToNextEdge();
            }
        }

        /**
         * @brief Оператор разыменования.
         * @return Индекс текущей вершины-соседа.
         */
        value_type operator*() const {
            return current_;
        }

        /**
         * @brief Префиксный инкремент итератора.
         * @return Ссылка на текущий объект итератора.
         */
        NeighborIterator& operator++() {
            ++current_;
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
            return current_ == other.current_ && rowPtr_ == other.rowPtr_;
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

    // Диапазон для удобства, чтобы можно было писать: 
    // for (auto end : graph.neighbors(begin)) { ... }

    /**
     * @brief Вспомогательный класс-диапазон для удобства итерирования.
     */
    class NeighborRange {
     private:
        const std::vector<std::optional<WeightType>>* rowPtr_{nullptr};

     public:
        /**
         * @brief Конструктор итератора.
         * @param rowPtr Указатель на строку матрицы смежности.
         */
        NeighborRange(const std::vector<std::optional<WeightType>>* rowPtr)
            : rowPtr_(rowPtr) {}

        /**
         * @brief Функция получения итератора на начало.
         * @return Итератор, который указывает на начало.
         */
        NeighborIterator begin() const {
            return NeighborIterator(rowPtr_, 0);
        }

        /**
         * @brief Функция получения итератора конца.
         * @return Итератор, который указывает за концом.
         */
        NeighborIterator end() const {
            if (!rowPtr_) return NeighborIterator();
            return NeighborIterator(rowPtr_, rowPtr_->size());
        }
    };

    // Метод для получения обычного итератора по соседям
    /**
     * @brief Получить обычный итератор по соседям.
     * @param end Данная вершина.
     * @return Получение диапазона итерирования.
     */
    NeighborRange neighbors(size_t end) const {
        return NeighborRange(&matrix_[end]);
    }

    /**
     * @brief Класс итератора по соседним вершинам, удовлетворяющих предикату по весу.
     * @tparam Predicate Тип предиката для фильтрации.
     */
    template <typename Predicate>
    class FilteredNeighborIterator {
     private:
        const std::vector<std::optional<WeightType>>* rowPtr_{nullptr};
        size_t current_{0};
        size_t rowSize_{0};
        [[no_unique_address]] Predicate pred_;

        void advanceToNextEdge() {
            while (current_ < rowSize_) {
                auto& optWeight = (*rowPtr_)[current_];
                // Проверяем, есть ли ребро, и удовлетворяет ли оно предикату
                if (optWeight.has_value() && pred_(current_, optWeight.value())) {
                    break;
                }
                ++current_;
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
        // FilteredNeighborIterator() = default;

        /**
         * @brief Конструктор итератора.
         * @param rowPtr Указатель на строку матрицы смежности.
         * @param startIndex Начальный индекс для итерации.
         * @param pred Тип предиката для фильтрации.
         */
        FilteredNeighborIterator(const std::vector<std::optional<WeightType>>* rowPtr,
                                 size_t startIndex, Predicate pred)
            : rowPtr_(rowPtr), current_(startIndex), pred_(pred) {
            if (rowPtr_) {
                rowSize_ = rowPtr_->size();
                advanceToNextEdge();
            }
        }

        /**
         * @brief Оператор разыменования.
         * @return Индекс текущей вершины-соседа.
         */
        value_type operator*() const {
            return current_;
        }

        /**
         * @brief Префиксный инкремент итератора.
         * @return Ссылка на текущий объект итератора.
         */
        FilteredNeighborIterator& operator++() {
            ++current_;
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
            return current_ == other.current_ && rowPtr_ == other.rowPtr_;
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
     private:
        const std::vector<std::optional<WeightType>>* rowPtr_{nullptr};
        Predicate pred_;
     public:
        /**
         * @brief Конструктор итератора.
         * @param rowPtr Указатель на строку матрицы смежности.
         * @param pred Тип предиката для фильтрации.
         */
        FilteredNeighborRange(const std::vector<std::optional<WeightType>>* rowPtr, Predicate pred)
            : rowPtr_(rowPtr), pred_(pred) {}

        /**
         * @brief Функция получения итератора на начало.
         * @return Итератор, который указывает на начало.
         */
        auto begin() const {
            return FilteredNeighborIterator<Predicate>(rowPtr_, 0, pred_);
        }

        /**
         * @brief Функция получения итератора конца.
         * @return Итератор, который указывает за концом.
         */
        auto end() const {
            // if (!rowPtr_) {
            //     return FilteredNeighborIterator<Predicate>();
            // }
            return FilteredNeighborIterator<Predicate>(rowPtr_, rowPtr_->size(), pred_);
        }
    };

    // Возвращает итератор, фильтрующий соседей по условию pred(begin, вес)
    // (где begin — индекс вершины-соседа).
    // Метод для получения обычного итератора по соседям
    /**
     * @brief Получить обычный итератор по соседям, фильтрующий соседей по условию pred(begin, вес).
     * @tparam Predicate Тип предиката для фильтрации.
     * @param end Данная вершина.
     * @param pred Тип предиката для фильтрации.
     * @return Получение диапазона итерирования.
     */
    template <typename Predicate>
    FilteredNeighborRange<Predicate> neighborsFiltered(size_t end, Predicate pred) const {
        return FilteredNeighborRange<Predicate>(&matrix_[end], pred);
    }
};