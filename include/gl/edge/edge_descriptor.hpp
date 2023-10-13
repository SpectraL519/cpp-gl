#pragma once

#include "gl/edge/edge_data.hpp"
#include "gl/utility.hpp"



namespace gl {

template <
    detail::u_integral VertexKey,
    detail::satisfies_or_void<detail::is_equality_comparable> Data
>
class edge_descriptor;



namespace edge {

template <typename Descriptor>
struct is_valid_descriptor : std::false_type {};

template <typename VertexKey, typename Data>
struct is_valid_descriptor <edge_descriptor <VertexKey, Data>> : std::true_type {};

template <typename Descriptor>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<Descriptor>::value;

} // namespace edge

template <typename Descriptor>
concept edge_descriptor_c = edge::is_valid_descriptor_v<Descriptor>;

template <typename Descriptor>
concept data_edge_descriptor_c = edge_descriptor_c<Descriptor> && data_descriptor_t<Descriptor>;

template <typename Descriptor, typename VertexKey>
concept key_type_matching_edge_descriptor_c =
    edge_descriptor_c<Descriptor> && std::is_same_v<typename Descriptor::vertex_key_type, VertexKey>;



template <
    detail::u_integral VertexKey = std::size_t,
    detail::satisfies_or_void<detail::is_equality_comparable> Data = void
>
class edge_descriptor {
public:
    using type = edge_descriptor<VertexKey, Data>;
    using vertex_key_type = VertexKey;
    using data_type = Data;


    edge_descriptor() = default;
    ~edge_descriptor() = default;

    explicit edge_descriptor(const vertex_key_type& source, const vertex_key_type& destination)
        : source(source), destination(destination)
    {}

    explicit edge_descriptor(
        const vertex_key_type& source, const vertex_key_type& destination,
        const data_type& data
    ) :
        source(source), destination(destination),
        _data(data)
    {}

    edge_descriptor(const type& other)
        : source(other.source), destination(other.destination),
          _data(other._data)
    {}

    edge_descriptor(type&& other)
        : source(other.source), destination(other.destination),
          _data(other._data)
    {}

    edge_descriptor(const edge_descriptor<VertexKey, void>& other)
        : source(other.source), destination(other.destination)
    {}

    edge_descriptor(const edge_descriptor<VertexKey, void>&& other)
        : source(other.source), destination(other.destination)
    {}


    friend bool operator==(const type& lhs, const type& rhs) {
        return lhs.source == rhs.source &&
               lhs.destination == rhs.destination &&
               lhs._data == rhs._data;
    }


    [[nodiscard]] inline const data_type& data() {
        return this->_data;
    }

    inline void set_data(const data_type& data) {
        this->_data = data;
    }

    [[nodiscard]] inline type reverse() {
        return type(this->destination, this->source, this->_data);
    }

    [[nodiscard]] inline type reverse(const data_type& reverse_data) {
        return type(this->destination, this->source, reverse_data);
    }


    const vertex_key_type source;
    const vertex_key_type destination;

private:
    data_type _data = data_type();
};



template <detail::u_integral VertexKey>
class edge_descriptor<VertexKey, void> {
public:
    using type = edge_descriptor<VertexKey, void>;
    using vertex_key_type = VertexKey;
    using data_type = void;


    edge_descriptor() = default;
    ~edge_descriptor() = default;

    explicit edge_descriptor(const vertex_key_type& source, const vertex_key_type& destination)
        : source(source), destination(destination)
    {}

    edge_descriptor(const type& other)
        : source(other.source), destination(other.destination)
    {}

    edge_descriptor(type&& other)
        : source(other.source), destination(other.destination)
    {}


    friend bool operator==(const type& lhs, const type& rhs) {
        return lhs.source == rhs.source &&
               lhs.destination == rhs.destination;
    }


    [[nodiscard]] inline type reverse() {
        return type(this->destination, this->source);
    }


    const vertex_key_type source;
    const vertex_key_type destination;
};

} // namespace gl
