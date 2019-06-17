package com.rusefi.config;

import com.rusefi.config.generated.Fields;

import java.util.HashMap;
import java.util.Map;

public class FieldsMap {
    public final static Map<String, Field> VALUES = new HashMap<>();

    static {
        for (Field field : Fields.VALUES)
            VALUES.put(field.getName(), field);
    }
}
