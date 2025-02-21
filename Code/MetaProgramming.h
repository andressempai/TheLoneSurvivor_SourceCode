#ifndef META_PROGRAMMING_H
#define META_PROGRAMMING_H

#include <type_traits>
#include <utility>
#include <tuple>

namespace MP {
	template <typename... Ts>
	struct type_list{};
	
	template <typename TypeListLike>
	struct size;
	
	template <typename TypeListLike>
	constexpr auto size_v = size<TypeListLike>::value;
	
	template <template <typename...> typename TypeListLike, typename... Ts>
	struct size<TypeListLike<Ts...>> {
		static constexpr auto value = sizeof...(Ts);
	};
	
	template <typename TypeListLike>
	struct is_empty : std::false_type {};
	
	template <typename TypeListLike>
	constexpr auto is_empty_v = is_empty<TypeListLike>::value;
	
	template <>
	struct is_empty<type_list<>> : std::true_type {};
	
	template <typename Element, typename List>
	struct has_type;
	
	template <typename Element, typename List>
	constexpr auto has_type_v = has_type<Element, List>::value;

	template <typename Element, template <typename...> class List, typename... Ts>
	struct has_type<Element, List<Ts...>> {
		static constexpr auto value = (false || ... || std::is_same_v<Element, Ts>);
	};
	
	template <typename Target, typename List>
	struct get_pos;

	template <typename Target, typename List>
	constexpr auto get_pos_v = get_pos<Target, List>::value;

	template <typename Target, template <typename...> typename TupleLike, typename... Ts>
	struct get_pos<Target, TupleLike<Target, Ts...>> {
		static constexpr auto value = 0;
	};

	template <typename Target, typename First, template <typename...> typename TupleLike, typename... Ts>
	struct get_pos<Target, TupleLike<First, Ts...>> {
		static constexpr auto value = 1 + get_pos<Target, TupleLike<Ts...>>::value;
	};
	
	template <std::size_t Number, typename List>
	struct get_nth_type;

	template <std::size_t Number, typename List>
	using get_nth_type_t = typename get_nth_type<Number, List>::type;

	template <typename Head, template <typename...> typename TupleLike, typename... Tail>
	struct get_nth_type<0, TupleLike<Head, Tail...>> {
		using type = Head;
	};

	template <std::size_t Number, typename Head, template <typename...> typename TupleLike, typename... Tail>
	struct get_nth_type<Number, TupleLike<Head, Tail...>> {
		using type = typename get_nth_type<Number - 1, TupleLike<Tail...>>::type;
	};

	template <typename List>
	struct front;
	
	template <typename List>
	using front_t = typename front<List>::type;	
	
	template <template <typename...> typename TupleLike, typename Head, typename... Tail>
	struct front<TupleLike<Head, Tail...>> {
		using type = Head;
	};

	template <typename List, typename Element>
	struct push_front;
	
	template <typename List, typename Element>
	using push_front_t = typename push_front<List, Element>::type;
	
	template <typename... TypesInList, typename Element>
	struct push_front<type_list<TypesInList...>, Element> {
		using type = type_list<Element, TypesInList...>;
	};
	
	template <typename List>
	struct pop_front;
	
	template <typename List>
	using pop_front_t = typename pop_front<List>::type;
	
	template <template <typename...> typename TupleLike, typename Head, typename... Tail>
	struct pop_front<TupleLike<Head, Tail...>> {
		using type = type_list<Tail...>;
	};
	
	template <typename List>
	struct back {
		using type = get_nth_type_t<(size<List>::value - 1), List>;
	};
	
	template <typename List>
	using back_t = typename back<List>::type;
	
	template <typename List, typename Element>
	struct push_back;
	
	template <typename List, typename Element>
	using push_back_t = typename push_back<List, Element>::type;
	
	template <typename... TypesInList, typename Element>
	struct push_back<type_list<TypesInList...>, Element> {
		using type = type_list<TypesInList..., Element>;
	};
	
	template <typename List, typename Element, bool Condition>
	struct push_back_with_condition;
	
	template <typename List, typename Element, bool Condition>
	using push_back_with_condition_t = typename push_back_with_condition<List, Element, Condition>::type;
	
	template <typename List, typename Element>
	struct push_back_with_condition<List, Element, false> {
		using type = List;   
	};
	
	template <typename List, typename Element>
	struct push_back_with_condition<List, Element, true> {
		using type = push_back_t<List, Element>;
	};
	
	template <typename InList, typename OutList, std::size_t N = size<InList>::value>
	struct pop_back;

	template <typename InList, typename OutList, std::size_t N = size<InList>::value>
	using pop_back_t = typename pop_back<InList, OutList, N>::type;

	template <typename Last, typename... TypesInList>
	struct pop_back<type_list<Last>, type_list<TypesInList...>, 1> {
		using type = type_list<TypesInList...>;
	};

	template <typename OutList, typename Head, typename... Tail, std::size_t N>
	struct pop_back<type_list<Head, Tail...>, OutList, N> {
		using out_in_list = type_list<Tail...>;
		using out_out_list = typename push_back<OutList, Head>::type;
		using type = typename pop_back<out_in_list, out_out_list, N - 1>::type;
	};

	template <std::size_t Min, std::size_t Max>
	struct make_sequence_range {
		template <std::size_t Begin, std::size_t... Indexes>
		static constexpr std::index_sequence<Begin + Indexes...> add(std::index_sequence<Indexes...>)
		{
			return {};
		}
	
		using type = decltype(add<Min>(std::make_index_sequence<Max-Min>()));
	};

	template <std::size_t Min, std::size_t Max>
	using make_sequence_range_t = typename make_sequence_range<Min, Max>::type;

	template <typename Sequence, typename InList>
	struct range_copy_list;

	template <typename Sequence, typename InList>
	using range_copy_list_t = typename range_copy_list<Sequence, InList>::type;
	
	template <std::size_t... Is, typename List>
	struct range_copy_list<std::index_sequence<Is...>, List> {
		using type = type_list<get_nth_type_t<Is, List>...>;
	};
	
	template <typename Destination, typename Origin>
	struct merge_list;
	
	template <typename Destination, typename Origin>
	using merge_list_t = typename merge_list<Destination, Origin>::type;
	
	template <typename... Ts>
	struct merge_list<type_list<Ts...>, type_list<>> {
		using type = type_list<Ts...>;
	};
	
	template <typename... TypesInDestinationList, typename... TypesInOriginList>
	struct merge_list<type_list<TypesInDestinationList...>, type_list<TypesInOriginList...>> {
		using destination = type_list<TypesInDestinationList...>;
		using origin = type_list<TypesInOriginList...>;
		using list_to_out = push_back_t<destination, front_t<origin>>;
		using list_to_merge = pop_front_t<origin>;
		using type = merge_list_t<list_to_out, list_to_merge>;
	};
	
	template <std::size_t Pos, typename Element, typename List>
	struct insert;
	
	template <std::size_t Pos, typename Element, typename List>
	using insert_t = typename insert<Pos, Element, List>::type;
	
	template <typename Element, typename... TypesInList>
	struct insert<0, Element, type_list<TypesInList...>> {
		using in_list = type_list<TypesInList...>;
		using type = push_front_t<in_list, Element>;
	};
	
	template <std::size_t Pos, typename Element, typename... TypesInList>
	struct insert<Pos, Element, type_list<TypesInList...>> {
		using in_list = type_list<TypesInList...>;
		using types_before_pos = range_copy_list_t<make_sequence_range_t<0, Pos>, in_list>;
		using types_from_pos_to_last = range_copy_list_t<make_sequence_range_t<Pos, size_v<in_list>>, in_list>;
		using types_before_pos_plus_new_element = push_back_t<types_before_pos, Element>;
		using type = merge_list_t<types_before_pos_plus_new_element, types_from_pos_to_last>;
	};
	
	template <std::size_t Pos, typename List>
	struct remove_at;
	
	template <std::size_t Pos, typename List>
	using remove_at_t = typename remove_at<Pos, List>::type;
	
	template <typename... Ts>
	struct remove_at<0, type_list<Ts...>> {
		using in_list = type_list<Ts...>;
		using type = pop_front_t<in_list>;
	};
	
	template <std::size_t Pos, typename... Ts>
	struct remove_at<Pos, type_list<Ts...>> {
		using in_list = type_list<Ts...>;
		using types_before_pos = range_copy_list_t<make_sequence_range_t<0, Pos>, in_list>;
		using types_after_pos = range_copy_list_t<make_sequence_range_t<Pos + 1, size_v<in_list>>, in_list>;
		using type = merge_list_t<types_before_pos, types_after_pos>;
	};
	
	template <typename T, typename TypeListLike, bool IsEmpty = is_empty_v<TypeListLike>>
	struct get_type_from_list;

	template <typename T, typename TypeListLike, bool IsEmpty = is_empty_v<TypeListLike>>
	using get_type_from_list_t = typename get_type_from_list<T, TypeListLike, IsEmpty>::type;

	template <typename T, template <typename...> typename TypeListLike>
	struct get_type_from_list<T, TypeListLike<>, true> {
		using type = void;
	};

	template <typename T, template <typename...> typename TypeListLike, typename... Ts>
	struct get_type_from_list<T, TypeListLike<Ts...>, false> {
		using list = TypeListLike<Ts...>;
		using head = front_t<list>;
		using tail = get_type_from_list_t<T, pop_front_t<list>>;
		using type = std::conditional_t<std::is_same<T, head>::value, head, tail>;
	};

	template <typename TypeListLike, template <typename> typename TypeMetaFunction>
	struct replace_each_type_in_list_with;

	template <typename TypeListLike, template <typename> typename TypeMetaFunction>
	using replace_each_type_in_list_with_t = typename replace_each_type_in_list_with<TypeListLike, TypeMetaFunction>::type;

	template <template <typename...> typename TypeListLike, template <typename> typename TypeMetaFunction, typename... Ts>
	struct replace_each_type_in_list_with<TypeListLike<Ts...>, TypeMetaFunction> {
		using type = TypeListLike<TypeMetaFunction<Ts>...>;
	};

	template <typename Component>
	struct get_tag {
		using type = typename Component::Tag;
	};

	template <typename Component>
	using get_tag_t = typename get_tag<Component>::type;

	template <typename TypeListLike>
	struct make_tuple_from_type_list_like;

	template <typename TypeListLike>
	using make_tuple_from_type_list_like_t = typename make_tuple_from_type_list_like<TypeListLike>::type;

	template <template <typename...> typename TypeListLike, typename... Ts>
	struct make_tuple_from_type_list_like<TypeListLike<Ts...>> {
		using type = std::tuple<Ts...>;
	};

	template <typename TypeList, template <typename...> class Target>
	struct transform_to_type_list_like;

	template <typename TypeList, template <typename...> class Target>
	using transform_to_type_list_like_t = typename transform_to_type_list_like<TypeList, Target>::type;

	template <template <typename...> class Source, template <typename...> class Target, typename... Ts>
	struct transform_to_type_list_like<Source<Ts...>, Target> {
		using type = Target<Ts...>;
	};

	template <typename Tuple, typename Rutine, typename... Ts>
	void for_each_tuple_using_types(Tuple& tuple, Rutine rutine, type_list<Ts...>)
	{
		static_assert((has_type<Ts, std::decay_t<decltype(tuple)>>::value and ...));
		(rutine.template operator()<Ts>(tuple), ...);
	}

	template <typename CallableObject, template <typename...> typename TypeListLike, typename... Ts>
	void for_each_type(CallableObject&& callable_object, TypeListLike<Ts...>)
	{
		(callable_object.template operator()<Ts>(), ...);
	}

	template <typename FromList, typename ToList, bool condition>
	struct copy_list_element_with_condition;

	template <typename FromList, typename ToList, bool condition>
	using copy_list_element_with_condition_t = typename copy_list_element_with_condition<FromList, ToList, condition>::type;

	template <typename ToList, bool condition>
	struct copy_list_element_with_condition<MP::type_list<>, ToList, condition> {
		using type = ToList;
	};

	template <typename OutList, bool condition, typename... Ts>
	struct copy_list_element_with_condition<MP::type_list<Ts...>, OutList, condition> {
		using FromList = MP::type_list<Ts...>;
		using FrontType = MP::front_t<FromList>;
		using type = copy_list_element_with_condition_t<
				MP::pop_front_t<FromList>
			,	std::conditional_t<condition, MP::push_back_t<OutList, FrontType>, OutList>
			,	condition
		>;
	};

	template <typename Component, template <typename> typename, typename = std::void_t<>>
	struct has_function : std::false_type {};

	template <typename Component, template <typename> typename Func>
	struct has_function<Component, Func, std::void_t<Func<Component>>> : std::true_type {};

	template <typename InList, typename OutList, template <typename> typename Function>
	struct copy_list_element_if_has_function;

	template <typename InList, typename OutList, template <typename> typename Function>
	using copy_list_element_if_has_function_t = typename copy_list_element_if_has_function<InList, OutList, Function>::type;

	template <typename OutList, template <typename> typename Function>
	struct copy_list_element_if_has_function<type_list<>, OutList, Function> {
		using type = OutList;
	};

	template <typename OutList, template <typename> class Function, typename... Ts>
	struct copy_list_element_if_has_function<type_list<Ts...>, OutList, Function> {
		using in_list = type_list<Ts...>;
		using head = front_t<in_list>;
		using type = copy_list_element_if_has_function_t<
				pop_front_t<in_list>
			,	push_back_with_condition_t<OutList, head, has_function<head, Function>::value>
			,	Function
		>;
	};

	template <template <typename> typename Func, typename OutList, typename... Ts>
	struct push_back_element_if_has_function;

	template <template <typename> typename Func, typename OutList, typename... Ts>
	using push_back_element_if_has_function_t = typename push_back_element_if_has_function<Func, OutList, Ts...>::type;

	template <template <typename> typename Func, typename OutList>
	struct push_back_element_if_has_function<Func, OutList> {
		using type = OutList;
	};

	template <template <typename> typename Func, typename OutList, typename Head, typename... Tail>
	struct push_back_element_if_has_function<Func, OutList, Head, Tail...> {
		using type = push_back_element_if_has_function_t<
				Func
			,	push_back_with_condition_t<OutList, Head, has_function<Head, Func>::value>
			,	Tail...
		>;
	};

	template <template <typename...> typename Conditional, typename OutList, typename... Ts>
	struct conditional_push_back_elements;

	template <template <typename...> typename Conditional, typename OutList, typename... Ts>
	using conditional_push_back_elements_t = typename conditional_push_back_elements<Conditional, OutList, Ts...>::type;

	template <template <typename...> typename Conditional, typename OutList>
	struct conditional_push_back_elements<Conditional, OutList> {
		using type = OutList;
	};

	template <template <typename...> typename Conditional, typename OutList, typename Head, typename... Tail>
	struct conditional_push_back_elements<Conditional, OutList, Head, Tail...> {
		using type = conditional_push_back_elements_t<
				Conditional
			,	push_back_with_condition_t<OutList, Head, Conditional<Head>::value>
			,	Tail...
		>;
	};

	template <
			template <typename...> typename Conditional
		,	typename OutList
		,	template <typename...> typename TypeListLike
		,	typename... Ts
	>
	struct conditional_push_back_elements<Conditional, OutList, TypeListLike<Ts...>> {
		using type = conditional_push_back_elements_t<Conditional, OutList, Ts...>;
	};
};	// !MP

#endif	// !META_PROGRAMMING_H