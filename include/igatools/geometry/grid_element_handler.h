//-+--------------------------------------------------------------------
// Igatools a general purpose Isogeometric analysis library.
// Copyright (C) 2012-2014  by the igatools authors (see authors.txt).
//
// This file is part of the igatools library.
//
// The igatools library is free software: you can use it, redistribute
// it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//-+--------------------------------------------------------------------

#ifndef GRID_ELEMENT_HANDLER_H_
#define GRID_ELEMENT_HANDLER_H_

#include <igatools/base/config.h>
#include <igatools/base/cache_status.h>
#include <igatools/base/new_flags_handler.h>
#include <igatools/base/quadrature.h>
#include <igatools/utils/tensor_product_array.h>
#include <igatools/geometry/cartesian_grid.h>

IGA_NAMESPACE_OPEN


template<std::size_t... I>
auto tuple_of_quads(std::index_sequence<I...>)
-> decltype(std::make_tuple(Quadrature<I>() ...))
{
    return std::make_tuple(Quadrature<I>() ...);
}

template<int dim>
using QuadList = decltype(tuple_of_quads(std::make_index_sequence<dim+1>()));

/**
 * Grid element value manager
 *
 * computational optimization cache, storing the interval length
 * in each direction.
 *
 */
template <int dim_>
class GridElementHandler
{
private:
    using GridType = CartesianGrid<dim_>;

protected:
    using ElementIterator = typename GridType::ElementIterator;
    using ElementAccessor = typename GridType::ElementAccessor;

protected:
    void init_element_cache(ElementAccessor &elem);
    void fill_element_cache(ElementAccessor &elem);
public:
    static const int dim = dim_;
    static const NewValueFlags valid_flags = NewValueFlags::measure |
                                             NewValueFlags::w_measure |
                                             NewValueFlags::point|
                                             NewValueFlags::length;

    //Allocates and fill the (global) cache
    GridElementHandler(std::shared_ptr<const GridType> grid);

    template<int k>
    void reset(const NewValueFlags flag, const Quadrature<k> &quad);

protected:
    template <int k>
    void fill_cache(ElementAccessor &elem, const int j);

    template <int k>
    void init_cache(ElementAccessor &elem);

    void init_all_caches(ElementAccessor &elem);

public:
    template <int k>
    void fill_cache(ElementIterator &elem, const int j)
    {
    	fill_cache<k>(elem.get_accessor(), j);
    }

    template <int k>
    void init_cache(ElementIterator &elem)
    {
    	init_cache<k>(elem.get_accessor());
    }

    void init_all_caches(ElementIterator &elem)
    {
        init_all_caches(elem.get_accessor());
    }

    /**
     * Allocates the space in ElementIterator element_cache
     * necessary for the given quadrature and flag combination.
     * It also fills the invariant (not changing) members of
     * the cache.
     */
    void init_element_cache(ElementIterator &elem);

    /**
     * Fills the ElementIterator element_cache
     * element dependent part
     */
    void fill_element_cache(ElementIterator &elem);


    void print_info(LogStream &out) const;

private:
    std::shared_ptr<const GridType> grid_;

    std::array<GridFlags, dim + 1> flags_;

protected:
    QuadList<dim> quad_;

    TensorProductArray<dim> lengths_;
};

IGA_NAMESPACE_CLOSE

#endif /* GRID_UNIFORM_QUAD_CACHE_H_ */
