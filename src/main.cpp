// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/denver_cug_web_scraping
//

#include <daw/curl_wrapper.h>
#include <daw/daw_algorithm.h>
#include <daw/daw_memory_mapped_file.h>
#include <daw/daw_string_view.h>
#include <daw/daw_unreachable.h>
#include <daw/gumbo_pp.h>
#include <daw/iterator/daw_find_iterator.h>
#include <daw/iterator/daw_find_one_iterator.h>
#include <daw/json/daw_json_link.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string_view>

namespace match = daw::gumbo::match;

void enumerate_all_div_tags( daw::gumbo::gumbo_range &doc_range,
                             daw::string_view html_doc ) {
	std::cout << "***** enumerate_all_div_tags *****\n";
	int count = 0;
	for( auto const &node : daw::find_iterator( doc_range.begin( ),
	                                            doc_range.end( ),
	                                            match::tag::DIV ) ) {
		std::cout << "  ***** div " << ( count++ ) << " *****\n";
		std::cout << daw::gumbo::node_inner_text( node, html_doc ) << '\n';
		std::cout << "  *****\n";
	}
	std::cout << "*****\n";
}

template<typename Keywords>
void find_all_links_with_keywords( daw::gumbo::gumbo_range &doc_range,
                                   Keywords &&keywords ) {
	std::cout << "***** find_all_links_with_keywords *****\n";
	for( auto const &node : daw::find_iterator(
	       doc_range.begin( ),
	       doc_range.end( ),
	       match::tag::A and
	         match::attribute::value::starts_with( "href", "http" ) and
	         match::content_text::contains( keywords ) ) ) {
		std::cout << "[" << daw::gumbo::node_content_text( node ) << ']';
		std::cout << "(" << daw::gumbo::node_attribute_value( node, "href" )
		          << ")\n";
	}
	std::cout << "*****\n";
}

void find_all_p_tags_with_id( daw::gumbo::gumbo_range &doc_range,
                              daw::string_view id ) {
	std::cout << "***** find_all_p_tags_with_id *****\n";
	for( auto const &node :
	     daw::find_iterator( doc_range.begin( ),
	                         doc_range.end( ),
	                         match::tag::P and match::id::is( id ) ) ) {
		std::cout << daw::gumbo::node_content_text( node ) << '\n';
	}
	std::cout << "*****\n";
}

struct Item {
	std::string title;
	std::string url;
	std::string price;
};

namespace daw::json {
	template<>
	struct json_data_contract<Item> {
		static constexpr char const title[] = "title";
		static constexpr char const url[] = "url";
		static constexpr char const price[] = "price";
		using type = json_member_list<json_string<title>,
		                              json_string<url>,
		                              json_string<price>>;

		static inline auto to_json_data( Item const &value ) {
			return std::forward_as_tuple( value.title, value.url, value.price );
		}
	};
} // namespace daw::json

void find_all_iphone13s( ) {
	auto crl = daw::curl_wrapper( );
	auto doc = crl.get_string( "https://www.newegg.ca/p/pl?d=iphone+13+pro" );
	auto doc_range = daw::gumbo::gumbo_range( doc );

	std::vector<Item> items{ };
	for( auto const &parent_node :
	     daw::find_iterator( doc_range.begin( ),
	                         match::class_type::is( "item-container" ) ) ) {
		auto child_range = daw::gumbo::gumbo_child_range( parent_node );
		auto child_filter_range = daw::find_one_iterator(
		  child_range.begin( ),
		  child_range.end( ),
		  match::tag::A and match::class_type::is( "item-title" ),
		  match::tag::LI and match::class_type::is( "price-current " ) );
		Item item{ };
		for( auto elem : child_filter_range ) {
			switch( elem.index ) {
			case 0:
				item.url = static_cast<std::string_view>(
				  daw::gumbo::node_attribute_value( elem.value, "href" ) );
				item.title = daw::gumbo::node_content_text( elem.value );
				break;
			case 1:
				item.price = daw::gumbo::node_content_text( elem.value );
				items.push_back( DAW_MOVE( item ) );
				break;
			}
		}
	}
	std::cout << daw::json::to_json( items ) << '\n';
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
	find_all_iphone13s( );
}
