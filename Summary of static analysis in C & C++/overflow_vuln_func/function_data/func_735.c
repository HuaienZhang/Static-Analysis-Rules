proto_item *
proto_tree_add_uint_bits_format_value(proto_tree *tree, const int hf_index,
				      tvbuff_t *tvb, const gint bit_offset,
				      const gint no_of_bits, guint32 value,
				      const char *format, ...)
{
	va_list ap;
	gchar  *dst;
	header_field_info *hf_field;

	TRY_TO_FAKE_THIS_ITEM(tree, hf_index, hf_field);

	switch (hf_field->type) {
		case FT_UINT8:
		case FT_UINT16:
		case FT_UINT24:
		case FT_UINT32:
			break;

		default:
			DISSECTOR_ASSERT_NOT_REACHED();
			return NULL;
			break;
	}

	CREATE_VALUE_STRING(dst, format, ap);

	return proto_tree_add_bits_format_value(tree, hf_index, tvb, bit_offset, no_of_bits, &value, dst);
}