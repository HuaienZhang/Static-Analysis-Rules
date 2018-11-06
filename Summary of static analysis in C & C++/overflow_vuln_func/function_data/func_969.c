static gint
hiqnet_display_data(proto_tree *hiqnet_payload_tree, tvbuff_t *tvb, gint offset) {
    guint8 datatype = 0;
    gint datalen;

    datatype = tvb_get_guint8(tvb, offset);
    proto_tree_add_item(hiqnet_payload_tree, hf_hiqnet_datatype, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset += 1;
    datalen = hiqnet_datasize_per_type[datatype];
    if (datalen < 0) { /
        datalen = tvb_get_ntohs(tvb, offset);
        proto_tree_add_item(hiqnet_payload_tree, hf_hiqnet_datalen, tvb, offset, 2, ENC_BIG_ENDIAN);
        offset += 2;
    }
    if (datatype == 9) { /
        proto_tree_add_item(hiqnet_payload_tree, hf_hiqnet_string, tvb, offset, datalen, ENC_UCS_2);
    } else {
        proto_tree_add_item(hiqnet_payload_tree, hf_hiqnet_value, tvb, offset, datalen, ENC_NA);
    }
    offset += datalen;
    return offset;
}