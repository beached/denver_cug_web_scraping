// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/denver_cug_web_scraping
//

#include <daw/daw_algorithm.h>
#include <daw/daw_memory_mapped_file.h>
#include <daw/daw_string_view.h>
#include <daw/gumbo_pp.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string_view>

namespace match = daw::gumbo::match;

void enumerate_all_div_tags( daw::gumbo::gumbo_range &doc_range,
                             daw::string_view html_doc ) {
	std::cout << "***** enumerate_all_div_tags *****\n";
	int count = 0;
	daw::algorithm::for_each_if(
	  doc_range.begin( ),
	  doc_range.end( ),
	  match::tag::DIV,
	  [&]( GumboNode const &node ) {
		  std::cout << "  ***** div " << (count++) << " *****\n";
		  std::cout << daw::gumbo::node_inner_text( node, html_doc ) << '\n';
			std::cout << "  *****\n";
	  } );
	std::cout << "*****\n";
}

template<typename Keywords>
void find_all_links_with_keywords( daw::gumbo::gumbo_range &doc_range,
                                  Keywords && keywords ) {
	std::cout << "***** find_all_links_with_keywords *****\n";
	daw::algorithm::for_each_if(
	  doc_range.begin( ),
	  doc_range.end( ),
	  match::tag::A and match::attribute::value::starts_with( "href", "http" ) and
	    match::content_text::contains( keywords ),
	  []( GumboNode const &node ) {
		  std::cout << "[" << daw::gumbo::node_content_text( node ) << ']';
		  std::cout << "(" << daw::gumbo::node_attribute_value( node, "href" )
		            << ")\n";
	  } );
	std::cout << "*****\n";
}

void find_all_p_tags_with_id( daw::gumbo::gumbo_range &doc_range,
                              daw::string_view id ) {
	std::cout << "***** find_all_p_tags_with_id *****\n";
	daw::algorithm::for_each_if( doc_range.begin( ),
	                             doc_range.end( ),
	                             match::tag::P and match::id::is( id ),
	                             []( GumboNode const &node ) {
		                             std::cout
		                               << daw::gumbo::node_content_text( node )
		                               << '\n';
	                             } );
	std::cout << "*****\n";
}

inline constexpr daw::string_view html_doc = R"html(
<html>
<head>
<title>Hello World</title>
<body>
<div>
<p id="foo">Foo Text</p><p id="bar">Bar Text</p>
<a href="https://www.google.ca">This is a foo text again</a>
<a href="https://www.google.ca">This is a bar text again</a>
<a href="https://www.google.ca">This might be foo text again</a>
</div>
<div>What?</div>
</body>
</html>
)html";

int main( ) {
	auto doc_range = daw::gumbo::gumbo_range(
	  std::string_view( html_doc.data( ), html_doc.size( ) ) );
	using namespace std::string_view_literals;
	enumerate_all_div_tags( doc_range, html_doc );
	find_all_links_with_keywords( doc_range, std::vector{ "foo"sv } );
	find_all_p_tags_with_id( doc_range, "foo" );
}
