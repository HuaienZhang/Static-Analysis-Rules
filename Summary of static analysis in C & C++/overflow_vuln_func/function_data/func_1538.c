static int vvalue_tvb_vector_internal(tvbuff_t *tvb, int offset, struct vt_vector *val, struct vtype_data *type, int num)
{
	const int offset_in = offset;
	const gboolean varsize = (type->size == -1);
	const int elsize = varsize ? (int)sizeof(struct data_blob) : type->size;
	guint8 *data = (guint8*)wmem_alloc(wmem_packet_scope(), elsize * num);
	int len, i;

	val->len = num;
	val->u.vt_ui1 = data;
	DISSECTOR_ASSERT((void*)&val->u == ((void*)&val->u.vt_ui1));

	for (i=0; i<num; i++) {
		DISSECTOR_ASSERT_HINT(type->tvb_get != 0,
				      "type that we don't know yet how to handle, please submit a bug with trace");
		len = type->tvb_get(tvb, offset, data);
		data += elsize;
		offset += len;
		if (varsize && (offset % 4) ) { /
			int padding = 4 - (offset % 4);
			offset += padding;
		}
	}
	return offset - offset_in;
}