                        proto_item *item)
{
    proto_item *array_tree;
    proto_item *sub;
    guint8      type;
    guint16     len16;
    const char *amqp_typename;
    const char *value;
    int         element_start;
    int         externally_formatted;
    guint32     element_count;
    guint32     struct_length;

    array_tree = 0;
    type = tvb_get_guint8(tvb, offset);
    AMQP_INCREMENT(offset, 1, bound);
    length -= 1;
    element_count = tvb_get_ntohl(tvb, offset);
    AMQP_INCREMENT(offset, 4, bound);
    length -= 4;
    proto_item_append_text(item, " (array of %d element%s)", element_count, element_suffix[element_count!=1]);
    if (element_count > 1)
        array_tree = proto_item_add_subtree(item, ett_amqp_0_10_array);
    while ((element_count > 0) && (length > 0)) {
        element_start = offset;
        externally_formatted = 0;
        switch (type) {
        case AMQP_0_10_TYPE_STR16:
            amqp_typename = "str16";
            len16 = tvb_get_ntohs(tvb, offset);
            AMQP_INCREMENT(offset, 2, bound);
            length -= 2;
            value   = (char*) tvb_get_string_enc(wmem_packet_scope(), tvb, offset, len16, ENC_UTF_8|ENC_NA);
            AMQP_INCREMENT(offset, len16, bound);
            length -= len16;
            break;

        case AMQP_0_10_TYPE_STRUCT32:
            amqp_typename = "struct32";
            value = "structure";
            externally_formatted = 1;
            struct_length = tvb_get_ntohl(tvb, offset);
            AMQP_INCREMENT(offset, 4, bound);
            length -= 4;
            array_tree = proto_item_add_subtree(item, ett_amqp_0_10_array);
            sub = proto_tree_add_none_format(array_tree, hf_amqp_field, tvb,
                                             element_start,
                                             offset - element_start,
                                             "(%s): ", amqp_typename);
            dissect_amqp_0_10_struct32(tvb, pinfo, sub, offset, struct_length);
            AMQP_INCREMENT(offset, struct_length, bound);
            length -= struct_length;
            break;

        default:
            proto_tree_add_none_format(array_tree, hf_amqp_field, tvb,
                                       element_start,
                                       offset - element_start,
                                       "(unknown type %d)",
                                       type);
            /
            return;
        }

        element_count -= 1;
        if (externally_formatted)
            continue;

        if (array_tree != 0) {
            proto_tree_add_none_format(array_tree, hf_amqp_field, tvb,
                                       element_start,
                                       offset - element_start,
                                       "(%s): %s",
                                       amqp_typename,
                                       value);
        }
        else {
            proto_item_append_text(item, ": (%s): %s", amqp_typename, value);
        }
    }
}