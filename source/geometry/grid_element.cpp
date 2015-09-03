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


#include <igatools/geometry/grid_element.h>
#include <igatools/geometry/unit_element.h>
#include <algorithm>

IGA_NAMESPACE_OPEN

template <int dim,bool GridIsConst>
GridElement<dim,GridIsConst>::
GridElement(const std::shared_ptr<ContainerType> &grid,
            const ListIt &index,
            const PropId &prop)
  :
  grid_(grid),
  property_(prop),
  index_it_(index)
{}





template <int dim,bool GridIsConst>
auto
GridElement<dim,GridIsConst>::
get_grid() const -> const std::shared_ptr<const ContainerType>
{
  return grid_;
}



template <int dim,bool GridIsConst>
auto
GridElement<dim,GridIsConst>::
get_index() const ->  const IndexType &
{
  return *index_it_;
}





template <int dim,bool GridIsConst>
bool
GridElement<dim,GridIsConst>::
has_property(const PropId &prop) const
{
  const auto &list = grid_->elem_properties_[prop];
  return std::binary_search(list.begin(), list.end(), get_index());
}



template <int dim,bool GridIsConst>
bool
GridElement<dim,GridIsConst>::
operator ==(const self_t &elem) const
{
  Assert(get_grid() == elem.get_grid(),
         ExcMessage("Cannot compare elements on different grid."));
  return (get_index() == elem.get_index());
}



template <int dim,bool GridIsConst>
bool
GridElement<dim,GridIsConst>::
operator !=(const self_t &elem) const
{
  Assert(get_grid() == elem.get_grid(),
         ExcMessage("Cannot compare elements on different grid."));
  return (get_index() != elem.get_index());
}

template <int dim,bool GridIsConst>
bool
GridElement<dim,GridIsConst>::
operator <(const self_t &elem) const
{
  Assert(get_grid() == elem.get_grid(),
         ExcMessage("Cannot compare elements on different grid."));
  return (get_index() < elem.get_index());
}

template <int dim,bool GridIsConst>
bool
GridElement<dim,GridIsConst>::
operator >(const self_t &elem) const
{
  Assert(get_grid() == elem.get_grid(),
         ExcMessage("Cannot compare elements on different grid."));
  return (get_index() > elem.get_index());
}




template <int dim,bool GridIsConst>
auto
GridElement<dim,GridIsConst>::
vertex(const int i) const -> Point
{
  Assert(i < UnitElement<dim>::sub_elements_size[0],
         ExcIndexRange(i,0, UnitElement<dim>::sub_elements_size[0]));

  TensorIndex<dim> index = get_index();

  auto all_elems = UnitElement<dim>::all_elems;
  const auto &vertex = std::get<0>(all_elems)[i];

  for (const auto j : UnitElement<dim>::active_directions)
  {
    index[j] += vertex.constant_values[j];
  }

  return grid_->knot_coordinates_.cartesian_product(index);
}



template <int dim,bool GridIsConst>
template <int sdim>
bool GridElement<dim,GridIsConst>::
is_boundary(const Index id) const
{
  const auto &n_elem = get_grid()->get_num_intervals();
  const auto &index = get_index();

  auto &sdim_elem = UnitElement<dim>::template get_elem<sdim>(id);

  for (int i = 0; i < dim-sdim; ++i)
  {
    auto dir = sdim_elem.constant_directions[i];
    auto val = sdim_elem.constant_values[i];
    if (((index[dir] == 0)               && (val == 0)) ||
        ((index[dir] == n_elem[dir] - 1) && (val == 1)))
      return true;
  }

  return false;
}



template <int dim,bool GridIsConst>
template <int sdim>
bool
GridElement<dim,GridIsConst>::
is_boundary() const
{
  for (auto &id : UnitElement<dim>::template elems_ids<sdim>())
    if (is_boundary<sdim>(id))
      return true;

  return false;
}




template <int dim,bool GridIsConst>
template <int sdim>
Real
GridElement<dim,GridIsConst>::
get_measure(const int s_id) const
{
  const auto lengths = get_side_lengths<sdim>(s_id);

  //  auto &sdim_elem = UnitElement<dim>::template get_elem<sdim>(j);

  Real measure = 1.0;
  for (int i=0; i<sdim; ++i)
    measure *= lengths[i];

  return measure;
}




template <int dim,bool GridIsConst>
template <int sdim>
ValueVector<Real>
GridElement<dim,GridIsConst>::
get_weights(const int s_id) const
{
  return this->template get_values_from_cache<_Weight, sdim>(s_id);
}



template <int dim,bool GridIsConst>
template <int sdim>
auto
GridElement<dim,GridIsConst>::
get_side_lengths(const int s_id) const -> const Points<sdim>
{
  Points<sdim> lengths;

  auto &s_elem = UnitElement<dim>::template get_elem<sdim>(s_id);

  int i=0;
  for (const int active_dir : s_elem.active_directions)
  {
    const auto &knots_active_dir = grid_->get_knot_coordinates(active_dir);
    const int j = get_index()[active_dir];
    lengths[i] = knots_active_dir[j+1] - knots_active_dir[j];
    ++i;
  }

  return lengths;
}



template <int dim,bool GridIsConst>
template <int sdim>
auto
GridElement<dim,GridIsConst>::
get_points(const int j) const ->ValueVector<Point>
{
  return this->template get_values_from_cache<_Point,sdim>(j);
}







template <int dim,bool GridIsConst>
auto
GridElement<dim,GridIsConst>::
get_element_points() const -> ValueVector<Point>
{
  return this->template get_points<dim>(0);
}


template <int dim,bool GridIsConst>
void
GridElement<dim,GridIsConst>::
print_info(LogStream &out) const
{
  out.begin_item("Property: ");
  out << property_ << std::endl;
  out.end_item();
  out.begin_item("Index:");
  index_it_->print_info(out);
  out.end_item();
}



template <int dim,bool GridIsConst>
void
GridElement<dim,GridIsConst>::
print_cache_info(LogStream &out) const
{
  if (all_sub_elems_cache_)
    all_sub_elems_cache_->print_info(out);
  else
    out << "Cache not allocated." << std::endl;
}



#if 0
template <int dim,bool GridIsConst>
SafeSTLVector<std::string>
GridElement<dim,GridIsConst>::
get_defined_properties() const
{
  SafeSTLVector<std::string> elem_properties;

  SafeSTLVector<std::string> grid_properties = grid_->properties_elements_id_.get_properties();
  for (const auto &property : grid_properties)
  {
    if (grid_->test_if_element_has_property(flat_index_, property))
      elem_properties.emplace_back(property);
  }
  return elem_properties;
}

#endif

#ifdef SERIALIZATION
template <int dim,bool GridIsConst>
template<class Archive>
void
GridElement<dim,GridIsConst>::
serialize(Archive &ar, const unsigned int version)
{
  using namespace boost::serialization;
  auto non_const_grid = std::const_pointer_cast<Grid<dim>>(grid_);
  ar &make_nvp("grid_",non_const_grid);
  grid_ = non_const_grid;
  Assert(grid_ != nullptr, ExcNullPtr());

  ar &make_nvp("property_", property_);
  Assert(false, ExcNotImplemented());
  //ar &make_nvp("index_it_", index_it_);
  ar &make_nvp("quad_list_", quad_list_);
  ar &make_nvp("all_sub_elems_cache_",all_sub_elems_cache_);
}
#endif // SERIALIZATION

IGA_NAMESPACE_CLOSE

#include <igatools/geometry/grid_element.inst>