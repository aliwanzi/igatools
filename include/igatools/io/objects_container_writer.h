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

#ifndef __OBJECTS_CONTAINER_WRITER_H_
#define __OBJECTS_CONTAINER_WRITER_H_

#include <igatools/base/config.h>

#include <set>

#ifdef XML_IO


IGA_NAMESPACE_OPEN

class XMLElement;
class XMLDocument;
class ObjectsContainer;
class IgCoefficients;

/**
 * @brief Helper class for writing an @ref ObjectsContainer into
 * a file with XML format.
 *
 * This class has a single public method, @ref write, that receives as
 * argument the @ref ObjectsContainer to be written and the destination file.
 *
 * Currently, this class is able to write the following classes:
 * - @ref Grid
 * - @ref SplineSpace
 * - @ref BSpline
 * - @ref NURBS
 * - @ref grid_functions::IdentityGridFunction
 * - @ref grid_functions::ConstantGridFunction
 * - @ref grid_functions::LinearGridFunction
 * - @ref IgGridFunction
 * - @ref Domain
 * - @ref PhysicalSpaceBasis
 * - @ref functions::ConstantFunction
 * - @ref functions::LinearFunction
 * - @ref IgFunction
 *
 * If any type different from the ones above is found, an exception
 * is thrown.
 *
 * The files written by using this class can be parsed with the
 * @ref ObjectsContainerParser class.
 *
 * The current igatools file format version is specified by static
 * variable @p IGATOOLS_FILE_FORMAT_VERSION of the class
 * @ref ObjectsContainerParser.
 *
 * The XML format is detailed here @subpage in_out.
 *
 * @see ObjectsContainer
 * @see ObjectsContainerWriter
 * @see XMLDocument
 * @see XMLElement
 *
 * @author P. Antolin
 * @date 2015
 */
class ObjectsContainerWriter
{
private:

  /** @name Types and static values */
  ///@{

  /** Type for the current class. */
  typedef ObjectsContainerWriter Self_;

  /** Type for a shared pointer of the current class. */
  typedef std::shared_ptr<Self_> SelfPtr_;

  /** Type for a shared pointer of @ref XMLDocument. */
  typedef std::shared_ptr<XMLDocument> XMLDocPtr_;

  /** Type for a shared pointer of @ref ObjectsContainer. */
  typedef std::shared_ptr<ObjectsContainer> ContPtr_;

  ///@}

  /** @name Constructors, destructor, assignment operators and creators */
  ///@{

  /**
   * @brief Default constructor.
   * @note Deleted, not allowed to be used.
   */
  ObjectsContainerWriter() = delete;

  /**
   * @brief Copy constructor.
   * @note Deleted, not allowed to be used.
   */
  ObjectsContainerWriter(const ObjectsContainerWriter &) = delete;

  /**
   * @brief Move constructor.
   * @note Deleted, not allowed to be used.
   */
  ObjectsContainerWriter(ObjectsContainerWriter &&) = delete;

  /**
   * @brief Copy assignment operator.
   * @note Deleted, not allowed to be used.
   */
  ObjectsContainerWriter &operator= (const ObjectsContainerWriter &) = delete;

  /**
   * @brief Move assignment operator.
   * @note Deleted, not allowed to be used.
   */
  ObjectsContainerWriter &operator= (ObjectsContainerWriter &&) = delete;

  ///@}

public:
  /**
   * @brief Writes the @p container to a @p file_path with XML format.
   *
   * @param[in] file_path Path of the file to be written.
   * @param[in] container Objects container to be written.
   */
  static void write(const std::string &file_path,
                    const ContPtr_ container);

private:

  /** @name Methods for writting the objects into the XML document */
  ///@{

  /**
   * @brief Appends all the @ref Grid
   * present in the @p container to the XML Document @p xml_doc.
   *
   * @param[in] container Container of the objects to be added.
   * @param[in] xml_doc XML Document where the objects are written to.
   */
  static void write_grids (const ContPtr_ container,
                           const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends all the @ref SplineSpace
   * present in the @p container to the XML Document @p xml_doc.
   *
   * @param[in] container Container of the objects to be added.
   * @param[in] xml_doc XML Document where the objects are written to.
   */
  static void write_spline_spaces (const ContPtr_ container,
                                   const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends all the @ref ReferenceSpaceBasis
   * present in the @p container to the XML Document @p xml_doc.
   *
   * @param[in] container Container of the objects to be added.
   * @param[in] xml_doc XML Document where the objects are written to.
   */
  static void write_reference_space_bases (const ContPtr_ container,
                                     const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends all the @ref GridFunction
   * present in the @p container to the XML Document @p xml_doc.
   *
   * @param[in] container Container of the objects to be added.
   * @param[in] xml_doc XML Document where the objects are written to.
   */
  static void write_grid_functions (const ContPtr_ container,
                                    const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends all the @ref Domain
   * present in the @p container to the XML Document @p xml_doc.
   *
   * @param[in] container Container of the objects to be added.
   * @param[in] xml_doc XML Document where the objects are written to.
   */
  static void write_domains (const ContPtr_ container,
                             const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends all the @ref PhysicalSpaceBasis
   * present in the @p container to the XML Document @p xml_doc.
   *
   * @param[in] container Container of the objects to be added.
   * @param[in] xml_doc XML Document where the objects are written to.
   */
  static void write_physical_space_bases (const ContPtr_ container,
                                          const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends all the @ref Function
   * present in the @p container to the XML Document @p xml_doc.
   *
   * @param[in] container Container of the objects to be added.
   * @param[in] xml_doc XML Document where the objects are written to.
   */
  static void write_functions (const ContPtr_ container,
                               const XMLDocPtr_ xml_doc);


  /**
   * @brief Appends a single @ref Grid
   * to the XML Document @p xml_doc.
   *
   * @tparam Grid Type of the @ref Grid to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class Grid>
  static void write_grid (const std::shared_ptr<Grid> grid,
                          const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref SplineSpace
   * to the XML Document @p xml_doc.
   *
   * @tparam SpSpace Type of the @ref SplineSpace to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class SpSpace>
  static void write_spline_space (const std::shared_ptr<SpSpace> spline_space,
                                  const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref BSpline
   * to the XML Document @p xml_doc.
   *
   * @tparam BSpline Type of the @ref BSpline to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class BSpline>
  static void write_bspline (const std::shared_ptr<BSpline> bspline,
                             const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref NURBS
   * to the XML Document @p xml_doc.
   *
   * @tparam NURBS Type of the @ref NURBS to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class NURBS>
  static void write_nurbs (const std::shared_ptr<NURBS> nurbs,
                           const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref grid_functions::IdentityGridFunction
   * to the XML Document @p xml_doc.
   *
   * @tparam IdGridFunc Type of the @ref grid_functions::IdentityGridFunction to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class IdGridFunc>
  static void write_identity_grid_function (const std::shared_ptr<IdGridFunc> id_func,
                                            const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref grid_functions::ConstantGridFunction
   * to the XML Document @p xml_doc.
   *
   * @tparam ConstGridFunc Type of the @ref grid_functions::ConstantGridFunction to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class ConstGridFunc>
  static void write_constant_grid_function (const std::shared_ptr<ConstGridFunc> const_func,
                                            const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref grid_functions::LinearGridFunction
   * to the XML Document @p xml_doc.
   *
   * @tparam LinearGridFunc Type of the @ref grid_functions::LinearGridFunction to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class LinearGridFunc>
  static void write_linear_grid_function (const std::shared_ptr<LinearGridFunc> linear_func,
                                          const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref IgGridFunction
   * to the XML Document @p xml_doc.
   *
   * @tparam IgGridFunc Type of the @ref IgGridFunction to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class IgGridFunc>
  static void write_ig_grid_function (const std::shared_ptr<IgGridFunc> ig_func,
                                      const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref Domain
   * to the XML Document @p xml_doc.
   *
   * @tparam Domain Type of the @ref Domain to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class Domain>
  static void write_domain (const std::shared_ptr<Domain> domain,
                            const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref PhysicalSpaceBasis
   * to the XML Document @p xml_doc.
   *
   * @tparam PhysSpaceBasis Type of the @ref PhysicalSpaceBasis to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class PhysSpaceBasis>
  static void write_phys_space_basis (const std::shared_ptr<PhysSpaceBasis> phys_space,
                                      const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref functions::ConstantFunction
   * to the XML Document @p xml_doc.
   *
   * @tparam ConstantFunction Type of the @ref functions::ConstantFunction to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class ConstantFunction>
  static void write_constant_function (const std::shared_ptr<ConstantFunction> const_function,
                                       const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref functions::LinearFunction
   * to the XML Document @p xml_doc.
   *
   * @tparam LinearFunc Type of the @ref functions::LinearFunction to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class LinearFunc>
  static void write_linear_function (const std::shared_ptr<LinearFunc> linear_func,
                                     const XMLDocPtr_ xml_doc);

  /**
   * @brief Appends a single @ref IgFunction
   * to the XML Document @p xml_doc.
   *
   * @tparam IgFunction Type of the @ref IgFunction to be appended.
   * @param[in] xml_doc XML Document where the object is appended to.
   */
  template <class IgFunction>
  static void write_ig_function (const std::shared_ptr<IgFunction> ig_function,
                                 const XMLDocPtr_ xml_doc);

  /**
   * @brief Creates a new @ref XMLElement containing
   * the passed @ref IgCoefficients.
   *
   * @param[in] coefs @ref IgCoefficients to the written into the element.
   * @param[in] xml_doc XML Document from where the element is created.
   */
  static std::shared_ptr<XMLElement>
  create_ig_coefs_xml_element(const IgCoefficients &coefs,
                              const XMLDocPtr_ xml_doc);

  ///@}

};

IGA_NAMESPACE_CLOSE

#endif // XML_IO

#endif // __OBJECTS_CONTAINER_WRITER_H_
