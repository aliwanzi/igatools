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

#ifndef IG_FUNCTIONS_H
#define IG_FUNCTIONS_H

#include <igatools/base/new_function.h>
#include <igatools/linear_algebra/distributed_vector.h>

IGA_NAMESPACE_OPEN

template<class Space>
class IgFunction :
        public NewFunction<Space::dim, Space::codim, Space::range, Space::rank>
{
public:
    static const int dim = Space::dim;
    static const int codim = Space::codim;
    static const int range = Space::range;
    static const int rank = Space::rank;

private:
    using base_t = NewFunction<dim, codim, range, rank>;
    using parent_t = NewFunction<dim, codim, range, rank>;
    using self_t = IgFunction<Space>;

public:
    using typename parent_t::variant_1;
    using typename parent_t::variant_2;
    using typename parent_t::Point;
    using typename parent_t::Value;
    using typename parent_t::Gradient;
    using typename parent_t::ElementIterator;
    using typename parent_t::ElementAccessor;
    template <int order>
    using Derivative = typename parent_t::template Derivative<order>;

    using CoeffType = vector<Real>;

    IgFunction(std::shared_ptr<const Space> space, const CoeffType &coeff);

public:
    static std::shared_ptr<base_t>
    create(std::shared_ptr<const Space> space, const CoeffType &coeff);


    std::shared_ptr<base_t> clone() const override
    {
        return std::make_shared<self_t>(self_t(*this));
    }

    IgFunction(const self_t &);

    void reset(const NewValueFlags &flag, const variant_1 &quad) override;

    void init_cache(ElementAccessor &elem, const variant_2 &k) override;

    void fill_cache(ElementAccessor &elem, const int j, const variant_2 &k) override;

    std::shared_ptr<const Space> get_iga_space() const;

    const CoeffType &get_coefficients() const;

    self_t &operator +=(const self_t &fun);

    void print_info(LogStream &out) const;

private:
    std::shared_ptr<const Space> space_;

    CoeffType coeff_;

    typename Space::ElementIterator elem_;

    typename Space::ElementHandler space_filler_;

private:
    struct ResetDispatcher : boost::static_visitor<void>
    {
        template<class T>
        void operator()(const T &quad)
        {
            (*flags_)[T::dim] = flag;
            space_handler_->template reset<T::dim>(flag, quad);
        }

        NewValueFlags flag;
        typename Space::ElementHandler *space_handler_;
        std::array<FunctionFlags, dim + 1> *flags_;
    };


    struct InitCacheDispatcher : boost::static_visitor<void>
    {
        template<class T>
        void operator()(const T &quad)
        {
            space_handler_->template init_cache<T::k>(*space_elem);
        }

        typename Space::ElementHandler  *space_handler_;
        typename Space::ElementAccessor *space_elem;


    };


    struct FillCacheDispatcher : boost::static_visitor<void>
    {
        template<class T>
        void operator()(const T &quad)
        {
            space_handler_->template fill_cache<T::k>(*space_elem, j);

            auto &local_cache = function->get_cache(*func_elem);
            auto &cache = local_cache->template get_value_cache<T::k>(j);
            auto &flags = cache.flags_handler_;

            if (flags.fill_values())
                std::get<0>(cache.values_) =
                    space_elem->template linear_combination<0, T::k>(*loc_coeff, j);
            if (flags.fill_gradients())
                std::get<1>(cache.values_) =
                    space_elem->template linear_combination<1, T::k>(*loc_coeff, j);
            if (flags.fill_hessians())
                std::get<2>(cache.values_) =
                    space_elem->template linear_combination<2, T::k>(*loc_coeff, j);

            cache.set_filled(true);
        }

        int j;
        self_t *function;
        typename Space::ElementHandler *space_handler_;
        ElementAccessor *func_elem;
        typename Space::ElementAccessor *space_elem;
        vector<Real> *loc_coeff;
    };

    ResetDispatcher reset_impl;
    InitCacheDispatcher init_cache_impl;
    FillCacheDispatcher fill_cache_impl;
};

IGA_NAMESPACE_CLOSE

#endif
