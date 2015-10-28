//-+--------------------------------------------------------------------
// Igatools a general purpose Isogeometric analysis library.
// Copyright (C) 2012-2015  by the igatools authors (see authors.txt).
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

#ifndef __GRID_FUNCTION_H_
#define __GRID_FUNCTION_H_

#include <igatools/base/config.h>
#include <igatools/geometry/grid.h>
#include <igatools/geometry/grid_handler.h>
#include <igatools/utils/shared_ptr_constness_handler.h>

IGA_NAMESPACE_OPEN

template <int, int, class> class GridFunctionElementBase;
template <int, int> class GridFunctionElement;
template <int, int> class ConstGridFunctionElement;
template <int, int> class GridFunctionHandler;

/**
 *
 */
template<int dim_, int space_dim_>
class GridFunction :
  public std::enable_shared_from_this<GridFunction<dim_,space_dim_> >
{
private:
  using self_t = GridFunction<dim_, space_dim_>;

public:
  static const int space_dim = space_dim_;
  static const int dim = dim_;

  using GridType = Grid<dim_>;

  using ElementAccessor = GridFunctionElement<dim_, space_dim_>;
  using ElementIterator = GridIterator<ElementAccessor>;
  using ConstElementAccessor = ConstGridFunctionElement<dim_, space_dim_>;
  using ElementConstIterator = GridIterator<ConstElementAccessor>;

  using ElementHandler = GridFunctionHandler<dim_, space_dim_>;

  using List = typename GridType::List;
  using ListIt = typename GridType::ListIt;

public:
  using GridPoint = typename GridType::Point;
  using Value = Values<dim, space_dim, 1>;
  template <int order>
  using Derivative = Derivatives<dim, space_dim, 1, order>;

  using Gradient = Derivative<1>;
  ///@}

  /**
   * Default constructor. It does nothing but it is needed for the
   * serialization mechanism.
   */
  GridFunction() = default;

  GridFunction(const SharedPtrConstnessHandler<GridType> &grid);


  virtual ~GridFunction() = default;




  std::shared_ptr<const GridType> get_grid() const;


  virtual std::unique_ptr<ElementHandler>
  create_cache_handler() const;

  std::unique_ptr<ConstElementAccessor>
  create_element(const ListIt &index, const PropId &prop) const;

  std::unique_ptr<ElementAccessor>
  create_element(const ListIt &index, const PropId &prop);

  ///@name Iterating of grid elements
  ///@{
  /**
   * This function returns a element iterator to the first element of the patch.
   */
  ElementIterator begin(const PropId &prop = ElementProperties::active);

  /**
   * This function returns a element iterator to one-pass the end of patch.
   */
  ElementIterator end(const PropId &prop = ElementProperties::active);

  /**
   * This function returns a element (const) iterator to the first element of the patch.
   */
  ElementConstIterator begin(const PropId &prop = ElementProperties::active) const;

  /**
   * This function returns a element (const) iterator to one-pass the end of patch.
   */
  ElementConstIterator end(const PropId &prop = ElementProperties::active) const;

  /**
   * This function returns a element (const) iterator to the first element of the patch.
   */
  ElementConstIterator cbegin(const PropId &prop = ElementProperties::active) const;

  /**
   * This function returns a element (const) iterator to one-pass the end of patch.
   */
  ElementConstIterator cend(const PropId &prop = ElementProperties::active) const;
  ///@}


  virtual void print_info(LogStream &out) const = 0;

#ifdef MESH_REFINEMENT
  std::shared_ptr<const self_t>
  get_grid_function_previous_refinement() const
  {
    return grid_function_previous_refinement_;
  }

  /**
   * Rebuild the internal state of the object after an insert_knots() function is invoked.
   *
   * @pre Before invoking this function, must be invoked the function grid_->insert_knots().
   * @note This function is connected to the Grid's signal for the refinement, and
   * it is necessary in order to avoid infinite loops in the insert_knots() function calls.
   *
   * @ingroup h_refinement
   */
  virtual void rebuild_after_insert_knots(
    const SafeSTLArray<SafeSTLVector<Real>,dim> &knots_to_insert,
    const Grid<dim> &old_grid) = 0;

  /**
   *  Connect a slot (i.e. a function pointer) to the refinement signals
   *  which will be
   *  emitted whenever a insert_knots() function is called by the underlying
   *  a Grid member.
   */
  boost::signals2::connection
  connect_insert_knots(const typename Grid<dim_>::SignalInsertKnotsSlot &subscriber);

  void create_connection_for_insert_knots(const std::shared_ptr<self_t> &grid_function);
#endif // MESH_REFINEMENT

private:
  SharedPtrConstnessHandler<Grid<dim_>> grid_;

  friend class GridFunctionElementBase<dim_, space_dim_, GridFunction<dim_, space_dim_>>;
  friend class GridFunctionElementBase<dim_, space_dim_, const GridFunction<dim_, space_dim_>>;
  friend class GridFunctionElement<dim_, space_dim_>;
  friend class ConstGridFunctionElement<dim_, space_dim_>;

#ifdef SERIALIZATION
  /**
   * @name Functions needed for serialization
   */
  ///@{
  friend class cereal::access;

  template<class Archive>
  void
  serialize(Archive &ar)
  {
    ar &make_nvp("grid_",grid_);
  }
  ///@}
#endif // SERIALIZATION

#ifdef MESH_REFINEMENT

protected:
  std::shared_ptr<const self_t> grid_function_previous_refinement_;
#endif // MESH_REFINEMENT

};

IGA_NAMESPACE_CLOSE

#endif

