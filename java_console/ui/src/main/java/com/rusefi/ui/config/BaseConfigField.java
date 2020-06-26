package com.rusefi.ui.config;

import com.opensr5.ConfigurationImage;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Field;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.UIContext;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.nio.ByteBuffer;

public abstract class BaseConfigField {
    protected final JLabel status = new JLabel("P");
    private final JPanel panel = new JPanel(new BorderLayout());
    private final UIContext uiContext;
    protected final Field field;

    public BaseConfigField(UIContext uiContext, final Field field) {
        this.uiContext = uiContext;
        this.field = field;
        status.setToolTipText("Pending...");
    }

    protected void requestInitialValue(final Field field) {
        ConnectionStatusLogic.INSTANCE.executeOnceConnected(() -> processInitialValue(field));
    }

    private void processInitialValue(Field field) {
        BinaryProtocol bp = uiContext.getLinkManager().getCurrentStreamState();
        if (bp == null)
            return;
        ConfigurationImage ci = bp.getControllerConfiguration();
        if (ci == null)
            return;
        loadValue(ci);
    }

    protected abstract void loadValue(ConfigurationImage ci);

    protected void onValueArrived() {
        status.setText("");
        status.setToolTipText(null);
    }

    protected void sendValue(Field field, String newValue) {
        String msg = field.setCommand() + " " + newValue;
        FileLog.MAIN.logLine("Sending " + msg);
        CommandQueue.getInstance().write(msg);
        status.setText("S");
        status.setToolTipText("Storing...");
    }

    protected void createUi(String topLabel, Component control) {
        JPanel center = new JPanel(new FlowLayout());

        control.setEnabled(false);

        /**
         * I guess a nice status enum is coming soon
         */
        center.add(status);

        center.add(control);

        panel.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.black),
                BorderFactory.createEmptyBorder(2, 2, 2, 2)));
        panel.add(new JLabel(topLabel), BorderLayout.WEST);
        panel.add(center, BorderLayout.CENTER);
    }

    public JPanel getContent() {
        return panel;
    }

    @NotNull
    protected ByteBuffer getByteBuffer(ConfigurationImage ci) {
        return field.getByteBuffer(ci);
    }

}