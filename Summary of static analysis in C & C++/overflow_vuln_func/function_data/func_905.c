static void
dissect_amqp_0_10_struct_queue_query_result(tvbuff_t *tvb,
                                            packet_info *pinfo,
                                            proto_item *tree,
                                            int offset,
                                            guint32 struct_length)
{
    proto_item *ti;
    proto_item *result;
    proto_item *flags_item;
    guint8      flag1, flag2;
    guint8      len8;
    guint32     map_length;
    int         flags_offset;
    int         max_length;

    max_length = offset + struct_length;
    result = proto_item_add_subtree(tree, ett_args);
    AMQP_INCREMENT(offset, 2, max_length);  /
    flags_offset = offset;
    flag1 = tvb_get_guint8(tvb, offset);
    flag2 = tvb_get_guint8(tvb, offset+1);
    flags_item = proto_tree_add_item(result,
                                     hf_amqp_0_10_argument_packing_flags,
                                     tvb, offset, 2, ENC_BIG_ENDIAN);

    if (flag2 != 0)
        expert_add_info(pinfo, flags_item, &ei_amqp_bad_flag_value);
    AMQP_INCREMENT(offset, 2, max_length);
    if (flag1 & 0x01) {
        /
        len8 = tvb_get_guint8(tvb, offset);
        THROW_ON(((offset + 1 + len8) > max_length), ReportedBoundsError);
        proto_tree_add_item(result,
                            hf_amqp_0_10_method_queue_name,
                            tvb, offset + 1, len8, ENC_ASCII|ENC_NA);
        AMQP_INCREMENT(offset, (1 + len8), max_length);
    }
    if (flag1 & 0x02) {     /
        len8 = tvb_get_guint8(tvb, offset);
        THROW_ON(((offset + 1 + len8) > max_length), ReportedBoundsError);
        proto_tree_add_item(result,
                            hf_amqp_0_10_method_queue_alt_exchange,
                            tvb, offset + 1, len8, ENC_ASCII|ENC_NA);
        AMQP_INCREMENT(offset, (1 + len8), max_length);
    }
    /
    proto_tree_add_item(result,
                        hf_amqp_0_10_struct_queue_query_result_durable,
                        tvb, flags_offset, 1, ENC_BIG_ENDIAN);
    proto_tree_add_item(result,
                        hf_amqp_0_10_struct_queue_query_result_exclusive,
                        tvb, flags_offset, 1, ENC_BIG_ENDIAN);
    proto_tree_add_item(result,
                        hf_amqp_0_10_struct_queue_query_result_auto_delete,
                        tvb, flags_offset, 1, ENC_BIG_ENDIAN);
    if (flag1 & 0x20) {     /
        map_length = tvb_get_ntohl(tvb, offset);
        AMQP_INCREMENT(offset, 4, max_length);
        THROW_ON(((offset + (int)map_length) > max_length), ReportedBoundsError);
        ti = proto_tree_add_item(result,
                                 hf_amqp_0_10_method_queue_declare_arguments,
                                 tvb,
                                 offset - 4,
                                 map_length + 4, ENC_NA);
        dissect_amqp_0_10_map (tvb,
                               offset,
                               offset + map_length,
                               map_length,
                               ti);
        AMQP_INCREMENT(offset, (int)map_length, max_length);
    }
    if (flag1 & 0x40) {     /
        proto_tree_add_item(result,
                            hf_amqp_0_10_struct_queue_query_result_message_count,
                            tvb, offset, 4, ENC_BIG_ENDIAN);
        AMQP_INCREMENT(offset, 4, max_length);
    }
    if (flag1 & 0x80) {     /
        proto_tree_add_item(result,
                            hf_amqp_0_10_struct_queue_query_result_subscriber_count,
                            tvb, offset, 4, ENC_BIG_ENDIAN);
        AMQP_INCREMENT(offset, 4, max_length);
    }
}