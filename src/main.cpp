// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/denver_cug_web_scraping
//

#include <daw/daw_algorithm.h>
#include <daw/daw_string_view.h>
#include <daw/gumbo_pp.h>

#include <algorithm>
#include <iostream>

namespace match = daw::gumbo::match;

void enumerate_all_div_tags( daw::gumbo::gumbo_range &doc_range,
                             daw::string_view html_doc ) {
	daw::algorithm::for_each_if(
	  doc_range.begin( ),
	  doc_range.end( ),
	  match::tag::DIV,
	  [&]( GumboNode const &node ) {
		  std::cout << daw::gumbo::node_inner_text( node, html_doc ) << '\n';
	  } );
}

template<std::size_t N>
void find_all_links_with_keyword( daw::gumbo::gumbo_range &doc_range,
                                  daw::string_view ( &keyword )[N] ) {
	daw::algorithm::for_each_if(
	  doc_range.begin( ),
	  doc_range.end( ),
	  match::tag::A and match::attribute::value::starts_with( "href", "http" ) and
	    match::content_text::contains( keyword ),
	  []( GumboNode const &node ) {
		  std::cout << "[" << daw::gumbo::node_content_text( node ) << ']';
		  std::cout << "(" << daw::gumbo::node_attribute_value( node, "href" )
		            << ")\n";
	  } );
}

void find_all_p_tags_with_id( daw::gumbo::gumbo_range &doc_range,
                              daw::string_view id ) {
	daw::algorithm::for_each_if( doc_range.begin( ),
	                             doc_range.end( ),
	                             match::tag::P and match::id::is( id ),
	                             []( GumboNode const &node ) {
		                             std::cout
		                               << daw::gumbo::node_content_text( node )
		                               << '\n';
	                             } );
}

int main( ) {}
