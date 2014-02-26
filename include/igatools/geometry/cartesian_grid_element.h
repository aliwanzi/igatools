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

#ifndef CARTESIAN_GRID_ELEMENT_H_
#define CARTESIAN_GRID_ELEMENT_H_

#include <igatools/base/config.h>
#include <igatools/base/tensor.h>
#include <igatools/geometry/cartesian_grid.h>

IGA_NAMESPACE_OPEN

template <int dim>
class CartesianGridElement
{
public:
    /** @name Constructors */
    ///@{
    /**
     * Default constructor. Not allowed to be used.
     */
    CartesianGridElement() = delete;

    /**
     * Construct an accessor pointing to the element with
     * flat index @p elem_index of the CartesianGrid @p grid.
     */
    CartesianGridElement(const CartesianGrid<dim> &grid,
                         const Index elem_index);

    /**
     * Copy constructor.
     */
    CartesianGridElement(const CartesianGridElement<dim> &elem)
        = default;

    /**
     * Move constructor.
     */
    CartesianGridElement(CartesianGridElement<dim> &&elem)
        = default;

    /**
     * Destructor.
     */
    ~CartesianGridElement() = default;
    ///@}

    /** @name Assignment operators */
    ///@{
    /**
     * Copy assignment operator. Not allowed to be used.
     */
    CartesianGridElement<dim>
    &operator=(const CartesianGridElement<dim> &elem) = default;

    /**
     * Move assignment operator. Not allowed to be used.
     */
    CartesianGridElement<dim>
    &operator=(CartesianGridElement<dim> &&elem) = default;
    ///@}



    /** Return the cartesian grid from which the element belongs.*/
    const CartesianGrid<dim> *get_grid() const;


    /** Functions related to the indices of the element in the cartesian grid. */
    ///@{
    /** Returns the index of the element in its flatten representation. */
    Index get_flat_index() const;

    /** Returns the index of the element in its tensor representation. */
    TensorIndex<dim>  get_tensor_index() const;


    /**
     * Sets the index of the element using the flatten representation.
     * @note This function also updates the index for the tensor representation.
     * @warning This may be a dangerous function, be careful when using it
     * as it is easy to use incorrectly. Only use it if you know what you
     * are doing.
     */
    void reset_flat_tensor_indices(const Index flat_index);


    /**
     * Sets the index of the element using the tensor representation.
     * @note This function also updates the index for the flatten representation.
     * @warning this may be a dangerous function, be careful when using it
     * as it is easy to use incorrectly. Only use it if you know what you
     * are doing.
     */
    void reset_flat_tensor_indices(const TensorIndex<dim> &tensor_index);
    ///@}


    /** @name Query geometrical/topological information without use of cache */
    ///@{
    /**
     * Return the @p i-th vertex
     */
    Point<dim> vertex(const int i) const;

    /**
     * Return the center of the element.
     */
    Point<dim> center() const;

    /**
     * Test if the point is inside the element.
     */
    bool is_point_inside(const Point<dim> &point) const;

    /**
     * Test if the element has a boundary face.
     */
    bool is_boundary() const;

    /**
     * Test if the face @p face on the current element is on the boundary of the cartesian grid.
     */
    bool is_boundary(const Index face_id) const;

    ///@}

private:

    /** Cartesian grid from which the element belongs.*/
    const CartesianGrid<dim> *grid_;

    /** Flat (linear) index assigned to the current (sub)-element. */
    Index flat_index_;

    /** Tensor product indices of the current struct index @p flat_index_. */
    TensorIndex<dim> tensor_index_;

};


IGA_NAMESPACE_CLOSE

#endif /* CARTESIAN_GRID_ELEMENT_H_ */