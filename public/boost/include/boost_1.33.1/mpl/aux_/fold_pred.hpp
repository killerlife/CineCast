
#ifndef BOOST_MPL_AUX_FOLD_PRED_HPP_INCLUDED
#define BOOST_MPL_AUX_FOLD_PRED_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: H:/cvsdata/public/boost_1_33_1/boost/mpl/aux_/fold_pred.hpp,v $
// $Date: 2006/06/29 08:51:43 $
// $Revision: 1.1.1.1 $

#include <boost/mpl/same_as.hpp>
#include <boost/mpl/apply.hpp>

namespace boost { namespace mpl { namespace aux {

template< typename Last >
struct fold_pred
{
    template<
          typename State
        , typename Iterator
        >
    struct apply
        : not_same_as<Last>::template apply<Iterator>
    {
    };
};

}}}

#endif // BOOST_MPL_AUX_FOLD_PRED_HPP_INCLUDED
